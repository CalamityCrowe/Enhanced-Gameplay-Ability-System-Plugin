
#pragma once

#include "CoreMinimal.h"
#include "AIStates.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMovementSpeed: uint8
{
	Idle UMETA(DisplayName = "Idle"), 
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running")
};


UENUM(BlueprintType)
enum class EAIStates: uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Investigating UMETA(DisplayName = "Investigating"),
	Attacking UMETA(DisplayName = "Attacking")
}; 

UENUM(BlueprintType)
enum class EAISenses: uint8 
{
	None UMETA(DisplayName = "None"),
	Sight UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Damage UMETA(DisplayName = "Damage")
};