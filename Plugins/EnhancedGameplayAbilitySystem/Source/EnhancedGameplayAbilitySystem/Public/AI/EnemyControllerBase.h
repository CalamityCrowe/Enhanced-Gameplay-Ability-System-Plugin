// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyControllerBase.generated.h"

class UBehaviorTree;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyControllerBase();
	
	virtual void StopBehaviourTree(); 
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Trees")
	TObjectPtr<UBehaviorTree> BehaviourTree;
};
