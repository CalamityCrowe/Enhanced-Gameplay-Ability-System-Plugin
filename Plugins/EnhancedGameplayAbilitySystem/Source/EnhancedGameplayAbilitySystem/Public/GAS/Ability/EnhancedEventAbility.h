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
	
	virtual void PostInitProperties() override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Animations", meta = (ToolTip = "This will be the montage associated with the ability. If we are having an event fire at a specific point in the montage, we would assign the anim notify in here"))
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (Categories = "Event",Tooltip = "baked within the montage of the animation, we will have an anim notify that will send an event to the owning character"))
	FGameplayTag EventTriggerTag;
	
	// this is what we will use to signal that we can extend the ability further after the montage has started to play
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnEventTriggered(FGameplayEventData Payload);  

	virtual void OnEventTriggered_Implementation(FGameplayEventData Payload);
};
