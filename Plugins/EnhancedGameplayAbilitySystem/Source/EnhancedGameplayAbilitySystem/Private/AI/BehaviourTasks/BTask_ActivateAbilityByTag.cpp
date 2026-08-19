// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviourTasks/BTask_ActivateAbilityByTag.h"

//engine
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"

// plugin
#include "Characters/CharacterBase.h"

UBTask_ActivateAbilityByTag::UBTask_ActivateAbilityByTag()
{
	NodeName = "ActivateAbilityByTag";
	bCreateNodeInstance = true;
	
}

// we check if the owning character is owning an Ability component,
// if so we will check all the activatable abilities they can have and check if the asset tags match the ability we are wanting to activate
EBTNodeResult::Type UBTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if (ACharacterBase* OwningCharacter = Cast<ACharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningCharacter); 
		if (!ASC) return EBTNodeResult::Failed;
		for (FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
		{
			if (Spec.Ability && Spec.Ability->GetAssetTags().HasTag(AbilityTag))
			{
				Handle = Spec.Handle; 
				if (ASC->TryActivateAbility(Handle))
				{
					ASC->AbilityEndedCallbacks.AddUObject(this, &UBTask_ActivateAbilityByTag::OnAbilityEnded); 
					OwnerCompRef = &OwnerComp;
					return EBTNodeResult::InProgress;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

// if for any reason the ability has been aborted, we want to unassign the call back and say that the task is done so we dont freeze the task node
EBTNodeResult::Type UBTask_ActivateAbilityByTag::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASC)
	{
		ASC->AbilityEndedCallbacks.RemoveAll(this); 
	}
	return EBTNodeResult::Aborted;
}

// since we will be working with an ability using the nodes, we will need this to essentially tell it that the task is done
void UBTask_ActivateAbilityByTag::OnAbilityEnded(UGameplayAbility* Ability)
{
	if (!Ability ||Ability->GetCurrentAbilitySpecHandle() != Handle)
	{
		return; 
	}
	
	if (ASC)
	{
		ASC->AbilityEndedCallbacks.RemoveAll(this);
	}
	
	FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded); 
}

FString UBTask_ActivateAbilityByTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s\n\nActivating Ability by Tag: %s"),*Super::GetStaticDescription(), *AbilityTag.ToString());
}
