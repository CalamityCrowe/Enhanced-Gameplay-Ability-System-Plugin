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
	
public: 
	virtual void NativeConstruct() override;
	
protected: 
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data); 
	virtual void UpdateStat(const UAbilitySystemComponent* ASC); 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> StatBar; 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CurrentStatText; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (ToolTip = "this is the attribute we want to listen for that will be the current value attributem i.e (Health, Mana, Stamina, etc)"))
	FGameplayAttribute TrackedAttribute; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (ToolTip = "this is the attribute we want to listen for that will be the max value of the current attribute we are listening for i.e (Max health, Max Mana, etc)")) 
	FGameplayAttribute TrackedMaxAttribute;
	
	float CurrentValue; 
	float MaxValue;
};
