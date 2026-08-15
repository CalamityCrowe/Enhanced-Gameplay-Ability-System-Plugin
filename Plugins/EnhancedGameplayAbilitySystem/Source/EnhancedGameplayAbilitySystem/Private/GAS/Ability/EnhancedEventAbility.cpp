// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/EnhancedEventAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UEnhancedEventAbility::UEnhancedEventAbility()
{
}

void UEnhancedEventAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// if we want the cost of an ability to be granted straight away along with the cost, we do it here as long as they have a valid cost and it is not on cooldown
	if (bShouldCommitOnActivation)
	{
		if (CheckCooldown(Handle, ActorInfo) == false || CheckCost(Handle, ActorInfo) == false)
		{
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return; 
		}		
		else
		{
			CommitAbility(Handle, ActorInfo,ActivationInfo); 
		}
	}
	else
	{
		if (CheckCooldown(Handle, ActorInfo) == false || CheckCost(Handle, ActorInfo) == false)
		{	
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
			return; 
		}	
	}
	
	// we will play the montage using an ability task, as this will handle all the clean-up correctly in the ability due to the delegates along with correct multiplayer setups
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName(""),
		AbilityMontage,
		1.0f,
		NAME_None
		); 
	
	
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UEnhancedEventAbility::OnMontageCompleted);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UEnhancedEventAbility::OnMontageCancelled);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UEnhancedEventAbility::OnMontageInterrupted);
	
	PlayMontageAndWait->ReadyForActivation();
	
	MontageStarted(); // we add this call here after the montage has started playing to avoid rewriting the entire montage activation
	
}

void UEnhancedEventAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEnhancedEventAbility::PostInitProperties()
{
	Super::PostInitProperties();
	
	EventTriggerTag = FGameplayTag::RequestGameplayTag(FName("Event.Montage.Triggered"), false); 
}

void UEnhancedEventAbility::MontageStarted_Implementation()
{
	// same as the OnEventTriggered. We won't implement anything here, but we can 
}

void UEnhancedEventAbility::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false); 
}

void UEnhancedEventAbility::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true); 
}

void UEnhancedEventAbility::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
}


void UEnhancedEventAbility::OnEventTriggered_Implementation(FGameplayEventData Payload)
{
}
