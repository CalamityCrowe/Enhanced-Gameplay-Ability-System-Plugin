// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnhancedStatBar.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UEnhancedStatBar::UEnhancedStatBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CurrentValue(50),MaxValue(100)
{
	
}



void UEnhancedStatBar::UpdateStat(const UAbilitySystemComponent* ASC)
{
	// we check if the ASC is valid before we grab the current tracked stats
	if (!ASC) return; 
	
	CurrentValue = ASC->GetNumericAttribute(TrackedAttribute);
	MaxValue = ASC->GetNumericAttribute(TrackedMaxAttribute);
	
	StatBar->SetPercent(CurrentValue/MaxValue); 
	
	if (CurrentStatText)
	{
		const FString StatText = FString::Printf(TEXT("%.0f"), CurrentValue);
		CurrentStatText->SetText(FText::FromString(StatText));
	}
}

void UEnhancedStatBar::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	// we will override this, as we will need the player state
}
