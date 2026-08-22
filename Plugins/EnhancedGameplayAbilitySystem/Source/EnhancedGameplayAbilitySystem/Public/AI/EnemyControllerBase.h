// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "States/AIStates.h"
#include "EnemyControllerBase.generated.h"

struct FAIStimulus;
class UBehaviorTree;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyControllerBase();
	
	virtual void StopBehaviourTree(); 
	UFUNCTION(BlueprintPure, Category = "AI", meta = (ToolTip = "Getting the current target focused on by the enemy\n\nPart of EnemyControllerBase"))
	AActor* GetCurrentTarget() const{return TargetActor.Get();}
	EAIStates GetCurrentState() const;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void UpdatePerception(const TArray<AActor*>& Actors);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Trees")
	TObjectPtr<UBehaviorTree> BehaviourTree;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Keys")
	FName AIStateKeyName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Keys")
	FName TargetKeyName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Keys")
	FName PointOfInterestKeyName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Keys")
	FName AttackRadiusKeyName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard|Keys")
	FName DefendRadiusKeyName;
	
	virtual void CanSenseActor(AActor* Actor, EAISenses SenseType, bool& OutSensed, FAIStimulus& OutStimulus); 
	
private: 
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerception;
		
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor;

	
	virtual void HandleSensedSight(AActor* Actor, const FAIStimulus& Stimulus);
	virtual void HandleSensedSound(const FVector& SoundLocation);
	
	void SetStateAsAttacking(AActor* Actor); 
	void SetStateAsInvestigating(const FVector& Location) const;
	
};
