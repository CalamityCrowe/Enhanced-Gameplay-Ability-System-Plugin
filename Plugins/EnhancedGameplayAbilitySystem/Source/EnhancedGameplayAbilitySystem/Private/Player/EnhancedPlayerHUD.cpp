// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnhancedPlayerHUD.h"

// plugin
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
		PlayerHUDWidget->AddToViewport();
	}
}
