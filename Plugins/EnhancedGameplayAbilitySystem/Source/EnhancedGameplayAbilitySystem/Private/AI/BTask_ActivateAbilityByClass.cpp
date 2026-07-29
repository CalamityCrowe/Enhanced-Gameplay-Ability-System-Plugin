// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTask_ActivateAbilityByClass.h"

//engine
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

//plugin
#include "Characters/CharacterBase.h"

UBTask_ActivateAbilityByClass::UBTask_ActivateAbilityByClass()
{
}

// instead of activating by an ability tag, we will activate the ability using a subclass of the ability
// this gives us more control over what is actually getting activated instead of leaving it to the wind with tags
EBTNodeResult::Type UBTask_ActivateAbilityByClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACharacterBase* OwningCharacter = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningCharacter); 
		if (!ASC) return EBTNodeResult::Failed;
		
		FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(AbilityToActivate); 
		AbilityHandle = AbilitySpec->Handle; 
		if (ASC->TryActivateAbility(AbilityHandle))
		{
			ASC->AbilityEndedCallbacks.AddUObject(this, &UBTask_ActivateAbilityByClass::OnAbilityEnded);
			OwnerCompRef = &OwnerComp; 
			return EBTNodeResult::InProgress; 
		}
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

// from here it's the standard method of aborting and finishing based on a handle
EBTNodeResult::Type UBTask_ActivateAbilityByClass::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	return EBTNodeResult::Aborted;
}

void UBTask_ActivateAbilityByClass::OnAbilityEnded(UGameplayAbility* Ability)
{
	if (!Ability || Ability->GetCurrentAbilitySpecHandle() != AbilityHandle)
	{
		return; 
	}
	if (ASC)
	{
		ASC->AbilityEndedCallbacks.RemoveAll(this); 	
	}
	FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded); 
	
}
