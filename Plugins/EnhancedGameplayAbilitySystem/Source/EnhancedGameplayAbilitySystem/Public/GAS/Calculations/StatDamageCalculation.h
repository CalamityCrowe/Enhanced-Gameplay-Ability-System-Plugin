// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "StatDamageCalculation.generated.h"

/**
 * 
 */


UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UStatDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public: 
	UStatDamageCalculation(); 
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
