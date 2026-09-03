// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayAbilitySpecHandle.h"
#include "BTask_ActivateAbilityByClass.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility; 
/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UBTask_ActivateAbilityByClass : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTask_ActivateAbilityByClass();
	
	protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void OnAbilityEnded(UGameplayAbility* Ability);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityToActivate; 
	
private: 
	
	// we mark these with UPROPERTY so the garbage collector doesn't remove them
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompRef;
	
	UPROPERTY()
	UAbilitySystemComponent* ASC; 
	
	UPROPERTY()
	FGameplayAbilitySpecHandle AbilityHandle; 
	
};
