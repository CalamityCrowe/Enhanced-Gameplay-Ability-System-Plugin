// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "BTask_ActivateAbilityByTag.generated.h"

/**
 * 
 */

class UAbilitySystemComponent; 
class UGameplayAbility;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UBTask_ActivateAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	UBTask_ActivateAbilityByTag(); 
	
	
	
protected:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void OnAbilityEnded(UGameplayAbility* Ability); 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Ability"), category = "Ability")
	FGameplayTag AbilityTag;
	
	virtual FString GetStaticDescription() const override;
private:
	
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompRef;
	
	UPROPERTY()
	UAbilitySystemComponent* ASC; 
	
	UPROPERTY()
	FGameplayAbilitySpecHandle Handle;
};
