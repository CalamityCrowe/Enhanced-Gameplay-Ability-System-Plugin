// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnhancedPlayerHUD.h"

// plugin
#include "AbilitySystemComponent.h"
#include "UI/EnhancedPlayerHUDWidget.h"

AEnhancedPlayerHUD::AEnhancedPlayerHUD()
{
	
}

void AEnhancedPlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (EnhancedPlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UEnhancedPlayerHUDWidget>(PlayerOwner,EnhancedPlayerHUDWidgetClass);
		PlayerHUDWidget->InitializeAttributeListening(CachedASC.Get()); 
		PlayerHUDWidget->AddToViewport();
	}
}

void AEnhancedPlayerHUD::SetAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("%s: Ability system component not valid"), *GetName())
#endif
		return;
	}
	CachedASC = InASC;
}
