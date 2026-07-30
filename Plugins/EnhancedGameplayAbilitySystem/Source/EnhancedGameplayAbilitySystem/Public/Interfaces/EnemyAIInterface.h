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
	
};
