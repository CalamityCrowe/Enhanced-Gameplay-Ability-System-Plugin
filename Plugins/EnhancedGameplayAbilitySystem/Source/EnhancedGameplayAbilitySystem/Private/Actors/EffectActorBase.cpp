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

// when a character is overlapping with the effect causer, we want to apply a gameplay effect here
// we store each actor that overlaps with the hazard in a map so we can keep track with what one is getting the effect removed later
void AEffectActorBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActiveEffects.Contains(OtherActor) || !EffectClass)return;
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
	{
		FGameplayEffectContextHandle EffectContext =  ASC->MakeEffectContext(); 
		EffectContext.AddSourceObject(this);
		EffectContext.AddHitResult(SweepResult, true); 
		FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectClass,1,EffectContext); // might update this later to generate a "world level" that updates the damage scalar
		EffectSpec.Data.Get()->SetSetByCallerMagnitude(EffectTag, EffectValue); 
		
		FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get()); 
		ActiveEffects.Add(OtherActor, ActiveEffectHandle);
	}
	
}

// how we handle the removal of the gameplay effect when they leave area
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

