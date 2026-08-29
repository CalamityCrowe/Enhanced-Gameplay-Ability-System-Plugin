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

void UEnhancedLevelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (const AEnhancedPlayerState* PS = GetOwningPlayerState<AEnhancedPlayerState>())
	{
		if (UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent())
		{
			CachedASC = ASC;
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
	if (Data.Attribute != UEnhancedAttributeSet::GetLevelAttribute())
	{
		FText TempLevelText = UTextFormattingHelper::FormatFloatWithCustomOptions(Data.NewValue, LevelTextFormat);
		LevelText->SetText(TempLevelText); 
	}
}
