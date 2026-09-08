// 

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EnhancedCombatAttributesSet.generated.h"

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedCombatAttributesSet : public UAttributeSet
{
	GENERATED_BODY()
public: 
	UEnhancedCombatAttributesSet(); 
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Attribute", ReplicatedUsing= OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedCombatAttributesSet, Strength)
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Attribute", ReplicatedUsing= OnRep_MaxStrengh)
	FGameplayAttributeData MaxStrength;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedCombatAttributesSet, MaxStrength)
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Attribute", ReplicatedUsing = OnRep_Defence)
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedCombatAttributesSet, Defence)
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat Attribute", ReplicatedUsing = OnRep_MaxDefence)
	FGameplayAttributeData MaxDefence;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedCombatAttributesSet, MaxDefence)
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldData);
	
	UFUNCTION()
	virtual void OnRep_MaxStrength(const FGameplayAttributeData& OldData);
	
	UFUNCTION()
	virtual void OnRep_Defence(const FGameplayAttributeData& OldData);

	UFUNCTION()
	virtual void OnRep_MaxDefence(const FGameplayAttributeData& OldData);
};
