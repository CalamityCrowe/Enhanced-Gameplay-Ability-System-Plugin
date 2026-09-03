// 

#include "UI/Components/EnhancedLevelWidget.h"

// engine
#include "Components/TextBlock.h"

// plugin
#include "GAS/Attributes/EnhancedAttributeSet.h"
#include "Player/EnhancedPlayerState.h"

UEnhancedLevelWidget::UEnhancedLevelWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

// similar idea as the status bar, we grab the ability system component and listen for when the level attribute changes and when it does it updates the text widget
void UEnhancedLevelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (const AEnhancedPlayerState* PS = GetOwningPlayerState<AEnhancedPlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
		{
			CachedASC = ASC; // caching the ASC so we can unbind the level change delegate handle
			FText TempLevelText = UTextFormattingHelper::FormatFloatWithCustomOptions(ASC->GetNumericAttribute(UEnhancedAttributeSet::GetLevelAttribute()), LevelTextFormat);
			LevelText->SetText(TempLevelText); 
			LevelChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(UEnhancedAttributeSet::GetLevelAttribute()).AddUObject(this, &UEnhancedLevelWidget::OnAttributeChanged); 
		}
	}
}

void UEnhancedLevelWidget::NativeDestruct()
{
	CachedASC->GetGameplayAttributeValueChangeDelegate(UEnhancedAttributeSet::GetLevelAttribute()).Remove(LevelChangedHandle);
	Super::NativeDestruct(); // do this at the end to prevent crashes due to it deleting this widget
}

void UEnhancedLevelWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (Data.Attribute == UEnhancedAttributeSet::GetLevelAttribute())
	{
		FText TempLevelText = UTextFormattingHelper::FormatFloatWithCustomOptions(Data.NewValue, LevelTextFormat);
		LevelText->SetText(TempLevelText); 
	}
}
