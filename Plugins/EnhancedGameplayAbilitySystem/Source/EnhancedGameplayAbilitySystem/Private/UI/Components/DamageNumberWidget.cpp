// 


#include "UI/Components/DamageNumberWidget.h"
#include "Components/TextBlock.h"

UDamageNumberWidget::UDamageNumberWidget(const FObjectInitializer& Initializer):Super(Initializer)
{
	
}

void UDamageNumberWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!FadeOutAnimation)return;
	PlayAnimationForward(FadeOutAnimation); 
}

void UDamageNumberWidget::SetDamageNumberText(const float InDamage) const
{
	const FString FormattedString = FString::Printf(TEXT("%.0f"), InDamage); 
	DamageText->SetText(FText::FromString(FormattedString));
}
