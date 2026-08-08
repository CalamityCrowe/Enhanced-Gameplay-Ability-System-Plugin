// 


#include "Actors/EffectActorBase.h"

//engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"

AEffectActorBase::AEffectActorBase(): EffectClass(nullptr), EffectTag(FGameplayTag()), EffectValue(0.0f)
{
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider")); 
	RootComponent = Collider;
	
	EffectParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect Particle")); 
	EffectParticle->SetupAttachment(RootComponent);
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap); 
	Collider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap); 
}

void AEffectActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActiveEffects.Contains(OtherActor) || !EffectClass)return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
	{
		FGameplayEffectContextHandle EffectContext =  ASC->MakeEffectContext(); 
		EffectContext.AddSourceObject(this);
		EffectContext.AddHitResult(SweepResult, true); 
		FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectClass,1,EffectContext); 
		EffectSpec.Data.Get()->SetSetByCallerMagnitude(EffectTag, EffectValue); 
		
		FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get()); 
		ActiveEffects.Add(OtherActor, ActiveEffectHandle);
	}
	
}

void AEffectActorBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor*  OtherActor,UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (FActiveGameplayEffectHandle* FoundHandle =  ActiveEffects.Find(OtherActor))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
		{
			ASC->RemoveActiveGameplayEffect(*FoundHandle);
		}
		ActiveEffects.Remove(OtherActor);
	}
}

