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
	
	virtual void PostInitProperties() override;
	
	EEnhancedActivationPolicy GetActivationPolicy() const{return ActivationPolicy; };
	
	bool ShouldShowInHUD() const {return bShouldShowInHUD;};
	
	UAbilityHUDData* GetHUDData() const {return AbilityHUDData; }
	
	FGameplayTag GetCooldownTag() const {return CooldownTag; };
	
	UFUNCTION(BlueprintPure)
	FGameplayTagContainer GetActivationOwnedTags() const;
	
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual float GetAbilityCooldown() const {return AbilityCooldown; };
	
	virtual float GetAbilityCost() const {return AbilityCost; };
	
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	virtual FGameplayTagContainer* GetCooldownTags() const override; 
	
	virtual UGameplayEffect* GetCooldownGameplayEffect() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability", meta = (ToolTip = "Increase the current ability level by a set amount passed in"))
	virtual void IncrementLevel(const int32 IncreaseValue = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability", meta = (ToolTip = "Decrease the current ability level by a set amount passed in"))
	virtual void DecrementLevel(const int32 DecreaseValue = 1);
	
	UFUNCTION(BlueprintCallable, Category= "Gameplay Ability", meta = (ToolTip = "Set the level of the current ability"))
	virtual void SetAbilityLevel(const int32 NewAbilityLevel); 
	
protected: 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	bool bHasCooldown;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (ClampMin = 1, UIMin = 1, ToolTip = "The minimum level the ability can be for the level"))
	int32 AbilityMinLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (ClampMin = 1, UIMin = 1, ToolTip = "The maximum level the ability can be for the level"))
	int32 AbilityMaxLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns", meta =(ToolTip = "This will be the tag we will be using to assign if the ability is on cooldown", EditCondition = "bHasCooldown", EditConditionHides))
	FGameplayTag CooldownTag; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns", meta = (ToolTip = "This will be the tag we will be using to assign for the cooldown duration of the ability", EditCondition = "bHasCooldown", EditConditionHides))
	FGameplayTag CooldownDurationTag; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns", meta = (ToolTip = "This is the value to associate with the cooldown, \n\n so if we set it to 2, we have 2 seconds before we can use the ability again", EditCondition = "bHasCooldown", EditConditionHides))
	float AbilityCooldown; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Cooldowns", meta = (ToolTip = "The gameplay effect we will be using instead to handle the cooldown instead", EditCondition = "bHasCooldown", EditConditionHides))
	TSubclassOf<UGameplayEffect> DynamicCooldownEffect; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Costs")
	float AbilityCost;
	
	// for this, we could substitute this to be like the GameplayCueTag container instead, where it has an additional check to see if the current tag is valid for the gameplay cue
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayCues", meta = (Categories = "GameplayCue"))
	FGameplayTag GameplayCue;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	EEnhancedActivationPolicy ActivationPolicy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (ToolTip = "We signal to the Ability component to actuvate this straight away"))
	bool bActivateOnGranted; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (ToolTip = "If we are making a UI system that has abilities displayed on screen,\n\n we set this to true so the UI can check this one value and grab the relevant HUD data from the AbilityHUDData file"))
	bool bShouldShowInHUD;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|HUD", meta = (EditCondition = "bShouldShowInHUD",EditConditionHides))
	TObjectPtr<UAbilityHUDData> AbilityHUDData;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilities")
	bool bShouldCommitOnActivation; 

private: 
	mutable FGameplayTagContainer TempCooldowntags;
};
