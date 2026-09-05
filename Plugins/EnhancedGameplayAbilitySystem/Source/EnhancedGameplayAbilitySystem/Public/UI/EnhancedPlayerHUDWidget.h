// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedPlayerHUDWidget.generated.h"

class UAbilitySystemComponent;
class UEnhancedLevelWidget;
class UEnhancedStatBar;
/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedPlayerHUDWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
	virtual void InitializeAttributeListening(UAbilitySystemComponent* InASC); 
	
protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UEnhancedStatBar> HealthBar; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UEnhancedStatBar> ShieldBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UEnhancedStatBar> EXPBar; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UEnhancedLevelWidget> LevelIcon;
};
