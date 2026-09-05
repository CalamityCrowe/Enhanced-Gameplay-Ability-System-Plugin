// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/EnhancedStatBar.h"

// engine
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/EnhancedPlayerState.h"

UEnhancedStatBar::UEnhancedStatBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CurrentValue(50),MaxValue(100)
{
	
}

// in the construction of the attribute, we want to bind the delegate for what attribute we are listening to 
void UEnhancedStatBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnhancedStatBar::NativeDestruct()
{
	if (CachedASC.IsValid())
	{
		CachedASC->GetGameplayAttributeValueChangeDelegate(TrackedAttribute).Remove(AttributeChangeHandle);
		CachedASC->GetGameplayAttributeValueChangeDelegate(TrackedMaxAttribute).Remove(MaxAttributeChangeHandle);
	}
	Super::NativeDestruct();
}

void UEnhancedStatBar::InitializeAttributeListening(UAbilitySystemComponent* InASC)
{
	if (!InASC || !TrackedAttribute.IsValid() || !TrackedMaxAttribute.IsValid())return;
	CachedASC = InASC;
	UpdateStat(CachedASC.Get());
	AttributeChangeHandle = CachedASC->GetGameplayAttributeValueChangeDelegate(TrackedAttribute).AddUObject(this, &ThisClass::OnAttributeChanged); 
	MaxAttributeChangeHandle = CachedASC->GetGameplayAttributeValueChangeDelegate(TrackedMaxAttribute).AddUObject(this, &ThisClass::OnAttributeChanged);
}

// within here, we will grab the current and max value of the attribute we are tracking and update the progress bar and text
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

// when the attribute changes, we will update the stat bar through this function bound to the delegate
void UEnhancedStatBar::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	UpdateStat(CachedASC.Get());
}
