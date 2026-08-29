
#include "UI/Enemy/EnhancedEnemyStatsWidget.h"

#include "UI/Components/EnemyStatBar.h"

UEnhancedEnemyStatsWidget::UEnhancedEnemyStatsWidget(const FObjectInitializer& Object):Super(Object)
{
	
}

void UEnhancedEnemyStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEnhancedEnemyStatsWidget::InitializeStats(UAbilitySystemComponent* InASC)
{
	if (!InASC) return; 
	HealthBar->InitializeStat(InASC); 
}
