// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI/States/AIStates.h"
#include "EnemyAIInterface.generated.h"

UINTERFACE()
class UEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};


class ENHANCEDGAMEPLAYABILITYSYSTEM_API IEnemyAIInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category="AI")
	void SetMovementSpeed(EMovementSpeed& SpeedMode, float& MovementSpeed);
	
	// this is how we get parameters as outputs instead by putting & after the typing
	// this means we are passing the type in as a reference, which means we can pass in values externally to get the values
	UFUNCTION(BlueprintNativeEvent, Category="AI")
	void GetIdealRange(float& OutAttackRange, float& OutDefendRange) const; 
};
