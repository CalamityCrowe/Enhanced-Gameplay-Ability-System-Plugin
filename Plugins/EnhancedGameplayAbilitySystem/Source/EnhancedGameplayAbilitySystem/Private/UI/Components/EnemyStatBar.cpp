// 


#include "UI/Components/EnemyStatBar.h"

#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"

UEnemyStatBar::UEnemyStatBar(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

void UEnemyStatBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnemyStatBar::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEnemyStatBar::InitializeStat(UAbilitySystemComponent* InASC)
{
	if (!InASC) return;
	ASCRef = InASC;
	UpdateStat(ASCRef);
	ASCRef->GetGameplayAttributeValueChangeDelegate(TrackedAttribute).AddUObject(this, &ThisClass::OnAttributeChanged); 
}

void UEnemyStatBar::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	UpdateStat(ASCRef); 
}

void UEnemyStatBar::UpdateStat(const UAbilitySystemComponent* ASC)
{
	if (!ASC) return; 
	
	CurrentValue = ASC->GetNumericAttribute(TrackedAttribute); 
	MaxValue = ASC->GetNumericAttribute(TrackedMaxAttribute); 
	
	StatBar->SetPercent(CurrentValue/MaxValue); 
}
