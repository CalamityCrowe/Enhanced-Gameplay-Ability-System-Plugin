// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "EnhancedStatBar.generated.h"

class UProgressBar;
class UTextBlock;
class UHorizontalBox; 
/**
 * 
 */
UCLASS(Abstract,HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedStatBar : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
protected: 
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data); 
	virtual void UpdateStat(const UAbilitySystemComponent* ASC); 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> StatBar; 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CurrentStatText; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	FGameplayAttribute TrackedAttribute; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	FGameplayAttribute TrackedMaxAttribute;
	
	float CurrentValue; 
	float MaxValue;
};
