// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnhancedGameplayAbility.generated.h"

/**
 * 
 */

class ACharacter;



UENUM(BlueprintType)
enum class EEnhancedActivationPolicy:uint8
{
	OnInputTriggered UMETA(DisplayName = "On Input Triggered"),
	WhileInputActive UMETA(DisplayName = "While Input Active"),
	Toggle UMETA(DisplayName = "Toggle Ability"), 
	None UMETA(Hidden)
};

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UAbilityHUDData: public UPrimaryDataAsset
{
	GENERATED_BODY()
public: 
	UAbilityHUDData()
	{
		DefaultColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		CooldownColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);
		ActiveColor = FLinearColor(1.0f, 1.0f, 0.0f, 0.5f);
	} 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Image Settings")
	FSlateBrush AbilityIcon; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Image Settings")
	FLinearColor DefaultColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Image Settings")
	FLinearColor ActiveColor; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Image Settings")
	FLinearColor CooldownColor;
	
	
};


UCLASS(Abstract , Blueprintable, meta= (HideFromSelection = true))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public: 
	
	UEnhancedGameplayAbility(); 
	EEnhancedActivationPolicy GetActivationPolicy() const{return ActivationPolicy; };
	
	UFUNCTION(BlueprintCallable,Category = "GAS|Ability")
	ACharacter* GetCharacterFromActorInfo() const; 
	
	UFUNCTION(BlueprintPure)
	FGameplayTagContainer GetActivationOwnedTags() const;
	
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual float GetAbilityCooldown() const {return AbilityCooldown; };
	
	virtual float GetAbilityCost() const {return AbilityCost; };
	
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	virtual FGameplayTagContainer* GetCooldownTags() const override; 
	
	virtual UGameplayEffect* GetCooldownGameplayEffect() const override;
	
protected: 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns")
	FGameplayTag CooldownTag; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns")
	FGameplayTag CooldownDurationTag; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns")
	float AbilityCooldown; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns")
	TSubclassOf<UGameplayEffect> DynamicCooldownEffect; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Costs")
	float AbilityCost;
	
	// for this, we could substitute this to be like the GameplayCueTag container instead, where it has an additional check to see if the current tag is valid for the gameplay cue
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayCues", meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	EEnhancedActivationPolicy ActivationPolicy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	bool bActivateOnGranted; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	bool bShouldShowInHUD;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	bool bShouldCommitOnActivation; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|HUD", meta = (EditCondition = "bShouldShowInHUD",EditConditionHides))
	TObjectPtr<UAbilityHUDData> AbilityHUDData;
	
private: 
	mutable FGameplayTagContainer TempCooldowntags;
};
