
#include "GAS/Calculations/SampleDamageCalculation.h"
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"

struct GSampleStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage); 
	
	GSampleStatics()
	{
		// we snapshot the damage in this instance as we want the value at that current moment the attack was made
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnhancedAttributeSet, Damage,Source, true); 
		
		// if we have other attributes we want to grab from different attribute sets, we can also define these 
		// to be grabbed here. for example, if we have a set for handling character stats like strength and defence, we can grab these to use as a modifier
	}
};

// this is a singleton, where we declare it within the CPP specifically. 
// the main difference here is we dont return it outside of this cpp file and it can only be used here specifically as it doesn't have a def in the header.
static const GSampleStatics& SampleStatics()
{
	static GSampleStatics Statics;
	return Statics;
}

USampleDamageCalculation::USampleDamageCalculation()
{
	RelevantAttributesToCapture.Add(SampleStatics().DamageDef); 
}

void USampleDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	
	AActor* TargetActor = TargetASC? TargetASC->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceASC? SourceASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec(); 
	
	// we grab the tags from both the target and source if we need to do anything specific to modify the damage
	// we use the aggregated tags to get all tags associated with the actor and the spec
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	bool bWarnIfNotFound = false; // error readability
	
	// next we want to get the damage that is getting passed in. if we have damage getting set by a caller, we will add that to the damage we captured
	float Damage = 0.0f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(SampleStatics().DamageDef, EvaluationParameters, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage"),bWarnIfNotFound),bWarnIfNotFound,0.0f),0.0f); 
	
	// from here, we can do any kind of calculations for stats
	float UnmitigatedDamage = Damage;
	
	float MitigatedDamage = (UnmitigatedDamage) * 0.5f; // for testing purposes we will just half the damage to get the point across
	
	// how we go about checking if they have a buff to mitigate damage
	
	
	if(TargetASC->GetOwnedGameplayTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Buff.Shield"))))
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,"Damage Negated"); 
		MitigatedDamage = 0.0f; // we set the damage to 0 to signal that we have a shield effect on
	}
	
	if (MitigatedDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(SampleStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage)); 
	}
}
