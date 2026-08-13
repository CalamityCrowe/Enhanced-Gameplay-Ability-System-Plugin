// 


#include "GAS/Ability/Melee/BaseUnarmedAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/Character.h"
#include "GAS/Data/SocketCollisionParamsObject.h"
#include "Kismet/KismetSystemLibrary.h"


UBaseUnarmedAbility::UBaseUnarmedAbility()
{
	constexpr bool bErrorIfFound = false; 
	HitScanStartTag = FGameplayTag::RequestGameplayTag(FName("Event.Hitscan.Start"), bErrorIfFound); 
	HitScanEndTag = FGameplayTag::RequestGameplayTag(FName("Event.Hitscan.End"), bErrorIfFound);
}

void UBaseUnarmedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// we want to exit here if we have no effect class to apply damage
	if (!EffectClass)
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue,FString::Printf(TEXT("Effect Class not set in: %s"), *GetClass()->GetName())); 	
#endif
		EndAbility(Handle, ActorInfo,ActivationInfo,true, true); 
		return;
	}
	
	// standard play montage and wait setup
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, MontageToPlay, 1.f); 
	
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted); 
	PlayMontageAndWait->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageBlendOut); 
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);
	PlayMontageAndWait->ReadyForActivation(); 
	
	// the main difference here is these events we are listening for. within the animation, we will bake a notify state 
	// that this will be listening for that will give the socket name and collision radius, whilst the other event will stop the hit scan to prevent constantly applying it
	UAbilityTask_WaitGameplayEvent* HitScanStartEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, HitScanStartTag, nullptr, false, true); 
	
	HitScanStartEvent->EventReceived.AddDynamic(this, &ThisClass::HitScanStarted);
	HitScanStartEvent->ReadyForActivation(); 
	
	UAbilityTask_WaitGameplayEvent* HitScanEndEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, HitScanEndTag, nullptr, false, true); 
	HitScanEndEvent->EventReceived.AddDynamic(this, &ThisClass::HitScanEnded);
	HitScanEndEvent->ReadyForActivation();
	
	MontageStarted(); 
	
}


void UBaseUnarmedAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false); 
}

void UBaseUnarmedAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UBaseUnarmedAbility::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

// when it recieves the hit scan to start tag, it will clear all hit actors form the list and grab the parameters from the payload
void UBaseUnarmedAbility::HitScanStarted(FGameplayEventData Payload)
{
	HitActors.Empty();
	if (const USocketCollisionParamsObject* Params = Cast<USocketCollisionParamsObject>(Payload.OptionalObject))
	{
		SocketName = Params->Params.SocketName;
		CollisionRadius = Params->Params.CollisionRadius;
	}
	
	if (GetWorld()->GetTimerManager().IsTimerActive(HitScanTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(HitScanTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(HitScanTimer,this, &ThisClass::HitScan, ScanInterval,true); 
}

// will stop performing the hit scan
void UBaseUnarmedAbility::HitScanEnded(FGameplayEventData Payload)
{
	GetWorld()->GetTimerManager().ClearTimer(HitScanTimer);
}

// the standard hit scan logic where we get the start location and perform a trace to get all the objects hit to apply the damage to them
void UBaseUnarmedAbility::HitScan()
{
	ACharacterBase* OwningCharacter = CastChecked<ACharacterBase>(GetOwningActorFromActorInfo()); 
	FVector SocketLocation = OwningCharacter->GetMesh()->GetSocketLocation(SocketName);
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetAvatarActorFromActorInfo()); 
	
	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), SocketLocation, SocketLocation, CollisionRadius, HitableObjectTypes,
		false, IgnoreActors,EDrawDebugTrace::ForDuration, HitResults, true, FLinearColor::Red, FLinearColor::Green,1.f);
	
	if (bHit)
	{
		for (FHitResult Hit: HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor)continue;
			UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
			if (!HitActors.Contains(HitActor) && ASC)
			{
				HitActors.AddUnique(HitActor);
				FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(EffectClass, 1);
				EffectSpecHandle.Data->SetSetByCallerMagnitude(EffectMagnitudeTag, EffectMagnitude); 
				
				EffectSpecHandle.Data->GetContext().AddHitResult(Hit,true); 
				ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get()); 
				
			}
			
		}
	}
	
}
