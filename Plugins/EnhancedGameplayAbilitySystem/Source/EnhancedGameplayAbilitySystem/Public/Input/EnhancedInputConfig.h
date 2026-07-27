// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputTriggers.h"
#include "GameplayTagContainer.h"
#include "EnhancedInputConfig.generated.h"

/**
 * 
 */

class UInputAction;

USTRUCT(Blueprintable)
struct FEnhancedInputAction
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (ToolTip = "This is the standard input action and hasn't been altred with a subclass"))
	TObjectPtr<UInputAction> InputAction; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (ToolTip = "This is the tag that will be used to say what ability the input belongs to, "))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (ToolTip = "We define the trigger type associated with the input. \n\nAs of writing this, it has only been handled for the Abiility inputs specifically, but there is a plan to add this in future versions"))
	ETriggerEvent TriggerType = ETriggerEvent::Started; 
	
};

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BLueprintReadOnly, Category = "Input", meta  = (ToolTip = "For the native inputs, these are inputs that not associated with abilities getting activated.\n\nSome of the inputs that have been defined already in the Enhanced Gameplay tags code, are the likes of movement, looking and jumping as these are all inputs that are normally associated with the character itself"))
	TArray<FEnhancedInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly, BLueprintReadOnly, Category = "Input", meta = (ToolTip = "For the ability inputs, these will be the inputs associated with abilities that will be getting granted to character"))
	TArray<FEnhancedInputAction> AbilityInputActions; 
	
	const UInputAction* FindNativeInputActionByTag(const FGameplayTag& InputTag) const; 
	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag) const;
	
};
