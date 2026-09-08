// 


#include "GAS/Attributes/EnhancedCombatAttributesSet.h"

#include "Net/UnrealNetwork.h"

UEnhancedCombatAttributesSet::UEnhancedCombatAttributesSet() :Strength(5.f), Defence(5.f)
{
}

void UEnhancedCombatAttributesSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedCombatAttributesSet, Strength,COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedCombatAttributesSet,MaxStrength,COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedCombatAttributesSet,Defence,COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedCombatAttributesSet,MaxDefence,COND_None, REPNOTIFY_Always); 
}

void UEnhancedCombatAttributesSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetMaxStrengthAttribute() || Attribute == GetMaxDefenceAttribute())
	{
		NewValue = FMath::RoundToInt32(FMath::Clamp(NewValue, 1.f, 255.f)); // these kind of attributes can't be a floating point number so we round it
	}  
}

void UEnhancedCombatAttributesSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UEnhancedCombatAttributesSet::OnRep_Strength(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedCombatAttributesSet, Strength, OldData); 
}

void UEnhancedCombatAttributesSet::OnRep_MaxStrength(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedCombatAttributesSet, MaxStrength, OldData); 
}

void UEnhancedCombatAttributesSet::OnRep_Defence(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedCombatAttributesSet, Defence, OldData); 
}

void UEnhancedCombatAttributesSet::OnRep_MaxDefence(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedCombatAttributesSet, MaxDefence, OldData); 
}
