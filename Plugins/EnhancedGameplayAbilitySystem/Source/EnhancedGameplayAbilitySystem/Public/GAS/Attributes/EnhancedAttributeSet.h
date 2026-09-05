// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EnhancedAttributeSet.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public: 
	UEnhancedAttributeSet(); 
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, Health); 
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth; 
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, MaxHealth);
	
	// this is a meta attribute that we will be using on damage calculations only. 
	// this doesn't need to be replicated as it will be a temporary value and only on the server
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage; 
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, Damage);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield; 
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, Shield)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxShield)
	FGameplayAttributeData MaxShield; 
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, MaxShield)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_XP)
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, XP)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxXP)
	FGameplayAttributeData MaxXP;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, MaxXP)
	
	// similar concept to the damage attribute, we don't need to replicate this as it is a meta attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData XPGained;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, XPGained)
	
	// this will be what we use to define how much exp we should give the character that has killed the enemy
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, XPBounty)
	
	UPROPERTY(BlueprintReadOnly, Category= "Attributes", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS_BASIC(UEnhancedAttributeSet, Level)
	
protected: 
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// this is applied before a gameplay effect is executed
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// after the effect is executed
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldData); 
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldData);
	
	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldData); 
	UFUNCTION()
	virtual void OnRep_MaxShield(const FGameplayAttributeData& OldData); 
	
	UFUNCTION()
	virtual void OnRep_XP(const FGameplayAttributeData& OldData); 
	UFUNCTION()
	virtual void OnRep_MaxXP(const FGameplayAttributeData& OldData); 
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldData); 

	// this is a recursive function where it will keep calling it, until it is no longer able to level
	virtual void TriggerLevelUp(ACharacterBase* Character); 
	
private: 
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
};
