
#pragma once

#include "CoreMinimal.h"

// We use this to define what direction the hit direction should be
UENUM(BlueprintType)
enum class EHitReactDirection:uint8
{
	None	UMETA(DisplayName = "None"), 
	Left	UMETA(DisplayName = "Left"),
	Front		UMETA(DisplayName = "Front"),
	Right	UMETA(DisplayName = "Right"),
	Back		UMETA(DisplayName = "Back"),
};