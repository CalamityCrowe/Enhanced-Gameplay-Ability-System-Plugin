// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnhancedPlayerHUDWidget.h"

#include "UI/Components/EnhancedLevelWidget.h"
#include "UI/Components/EnhancedStatBar.h"

UEnhancedPlayerHUDWidget::UEnhancedPlayerHUDWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

void UEnhancedPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnhancedPlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEnhancedPlayerHUDWidget::InitializeAttributeListening(UAbilitySystemComponent* InASC)
{
	if (HealthBar) HealthBar->InitializeAttributeListening(InASC); 
	if (ShieldBar) ShieldBar->InitializeAttributeListening(InASC);
	if (EXPBar) EXPBar->InitializeAttributeListening(InASC);
	if (LevelIcon) LevelIcon->InitializeAttributeListening(InASC);
}
