// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EnhancedPlayerHUD.generated.h"

class UAbilitySystemComponent;
class UEnhancedPlayerHUDWidget;
/**
 * 
 */
UCLASS(Abstract, meta = (HideFromSelection = true))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnhancedPlayerHUD : public AHUD
{
	GENERATED_BODY()
public: 
	AEnhancedPlayerHUD(); 
	
	virtual void BeginPlay() override; 
	
	virtual void SetAbilitySystemComponent(UAbilitySystemComponent* InASC);
protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (ToolTip = "We assign the sub class of the player HUD widget here to display current stats on the screen like the player health and such "))
	TSubclassOf<UEnhancedPlayerHUDWidget> EnhancedPlayerHUDWidgetClass;
	
	UPROPERTY()
	UEnhancedPlayerHUDWidget* PlayerHUDWidget; 
	
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	
};
