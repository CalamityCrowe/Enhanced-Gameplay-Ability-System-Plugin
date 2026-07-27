// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EnhancedAbilitySystemComponent.generated.h"

/**
 * 
 */

class UEnhancedGameplayAbility; 

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public: 
	
	UEnhancedAbilitySystemComponent(); 
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag); 
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused); 
	void ClearAbilityInput(); 
	
	bool IsAbilityActive(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags, UEnhancedGameplayAbility* Ignore = nullptr); 
	
	bool IsAbilityActive(TSubclassOf<UEnhancedGameplayAbility> AbilityClass); 
	
protected: 
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	
private: 
	
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
};
