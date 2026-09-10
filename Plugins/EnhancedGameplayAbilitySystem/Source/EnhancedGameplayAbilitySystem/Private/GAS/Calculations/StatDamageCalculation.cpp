// 


#include "GAS/Calculations/StatDamageCalculation.h"

#include "Curves/BezierUtilities.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"
#include "GAS/Attributes/EnhancedCombatAttributesSet.h"

struct GStatStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defence); 
	
	// for the snapshotting of the value, we do this to define how the effect treats the damage we are applying 
	// for true, if the damage is set to 500 at the time of creating the effect, but is later adjusted to 800 it will keep the 500 for the damage
	GStatStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnhancedAttributeSet,Damage, Source, true); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnhancedCombatAttributesSet,Strength, Source, false); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnhancedCombatAttributesSet, Defence,Target, false); 
	}
};


static const GStatStatics& StatsStatics()
{
	static GStatStatics Statics;
	return Statics;
}

UStatDamageCalculation::UStatDamageCalculation()
{
	RelevantAttributesToCapture.Add(StatsStatics().DamageDef); 
	RelevantAttributesToCapture.Add(StatsStatics().StrengthDef); 
	RelevantAttributesToCapture.Add(StatsStatics().DefenceDef);
}

void UStatDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent(); 
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceActor = SourceASC? SourceASC->GetAvatarActor():nullptr;
	const AActor* TargetActor = TargetASC? TargetASC->GetAvatarActor():nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec(); 
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags(); 
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Strength = 0.0f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StatsStatics().StrengthDef, EvaluationParameters, Strength);
	Strength = FMath::Max<float>(Strength, 0.0f); // makes sure that the strength attribute is positive
	
	float Defence = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StatsStatics().DefenceDef, EvaluationParameters, Defence);
	Defence = FMath::Max<float>(Defence, 0.0f);

	constexpr bool bWarnIfNotFound = false;
	
	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(StatsStatics().DamageDef, EvaluationParameters, Damage);
	Damage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), bWarnIfNotFound,0.0f),0.0f);
	
	float FinalDamage = Strength * (Damage/(Damage + Defence));
	
	if (TargetASC->GetOwnedGameplayTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Buff.Shield"),bWarnIfNotFound)))
	{
		FinalDamage = 0.0f;
	}
	
	if (FinalDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(StatsStatics().DamageProperty,EGameplayModOp::Additive,FinalDamage)); 
	}
	
}
