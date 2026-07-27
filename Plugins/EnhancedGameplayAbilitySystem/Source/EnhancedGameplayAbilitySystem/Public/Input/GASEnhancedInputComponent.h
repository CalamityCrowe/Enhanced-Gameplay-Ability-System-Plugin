// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "EnhancedInputConfig.h"
#include "GASEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UGASEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public: 
	UGASEnhancedInputComponent(); 
	
	///  we make these template functions so it can take any parameter that can be considered a valid object that can be passed into the 
	/// @tparam UserClass  in this instance, we would normally pass "this"  as the argument for the userclass for this typically refers to the owning class
	/// @tparam FuncType  in this instance we can either pass a created function or a lambda function so it can bind these to the actions along with the trigger event
	/// 
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UEnhancedInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEVent, UserClass* Object, FuncType Func); 
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UEnhancedInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
inline void UGASEnhancedInputComponent::BindNativeAction(const UEnhancedInputConfig* InputConfig, const FGameplayTag& InputTag,
                                                         ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionByTag(InputTag) )
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGASEnhancedInputComponent::BindAbilityActions(const UEnhancedInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	for (const FEnhancedInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction,Action.TriggerType,Object,PressedFunc,Action.InputTag).GetHandle());
			}
			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction,ETriggerEvent::Completed,Object,ReleasedFunc,Action.InputTag ).GetHandle());
			}
		}
	}
}