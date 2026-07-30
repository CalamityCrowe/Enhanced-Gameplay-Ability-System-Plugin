// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyControllerBase.h"

//engine
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gameframework/Character.h"
#include "Kismet/GameplayStatics.h"

AEnemyControllerBase::AEnemyControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyControllerBase::StopBehaviourTree()
{
	if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple,"Behavior tree stopped running"); 
#endif
		BTComp->StopTree(); 
	}
}

void AEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviourTree)
	{
		RunBehaviorTree(BehaviourTree);
	}
}


