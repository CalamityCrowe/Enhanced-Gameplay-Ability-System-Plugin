// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedGameplayAbility.h"
#include "EnhancedEventAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedEventAbility : public UEnhancedGameplayAbility
{
	GENERATED_BODY()
	
public: 
	
	UEnhancedEventAbility(); 
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Animations")
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FGameplayTag EventTriggerTag;
	
	// this is what we will use to signal that we can extend the ability firther after the montage has started to play
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MontageStarted(); 
	
	// we use this variation to implement it further in c++ 
	virtual void MontageStarted_Implementation();
	
	UFUNCTION()
	virtual void OnMontageCompleted(); 
	
	UFUNCTION()
	virtual void OnMontageCancelled(); 
	
	UFUNCTION()
	virtual void OnMontageInterrupted(); 
	
	// this is the function we will override if we have an event to trigger
	UFUNCTION()
	virtual void OnEventTriggered(FGameplayEventData Payload);  
};
