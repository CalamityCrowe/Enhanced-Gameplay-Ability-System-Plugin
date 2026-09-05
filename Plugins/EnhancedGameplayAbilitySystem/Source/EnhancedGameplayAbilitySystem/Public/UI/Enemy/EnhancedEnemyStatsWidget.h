// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedEnemyStatsWidget.generated.h"


class UEnhancedStatBar;
class UAbilitySystemComponent;
class UHorizontalBox;
class USizeBox;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedEnemyStatsWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	virtual void NativeConstruct()	override; 
	
	virtual void InitializeStats(UAbilitySystemComponent* InASC); 
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> WidgetSize;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> WidgetContainer; 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatsContainer;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UEnhancedStatBar> HealthBar;
	
};
