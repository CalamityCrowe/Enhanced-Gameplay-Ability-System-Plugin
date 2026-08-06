#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SampleDamageCalculation.generated.h"

/**
 * since this is a sample class to show how to implement these within an actual framework
 * 
 * most of how this is setup will be in the cpp as we need to create a struct to handle these properly
 * 
 * this class will also be marked as abstract and hidedropdown to prevent it from being created by mistake.
 */
UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API USampleDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public: 
	USampleDamageCalculation();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
