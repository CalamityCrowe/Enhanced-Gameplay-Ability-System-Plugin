// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/EnhancedInputConfig.h"

// we loop through all the native inputs and check if the tag associated with the input is found  and if so we return the input action
const UInputAction* UEnhancedInputConfig::FindNativeInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FEnhancedInputAction& Action: NativeInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	return nullptr; 
}

// we loop through all the ability inputs and check if the tag associated with the input is found and if so, we return the input action
const UInputAction* UEnhancedInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FEnhancedInputAction& Action: AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	return nullptr; 
}
