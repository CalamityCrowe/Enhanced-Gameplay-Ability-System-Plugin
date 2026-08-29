// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "EnemyStatBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnemyStatBar : public UUserWidget
{
	GENERATED_UCLASS_BODY()
	
public: 
	virtual void NativeConstruct()override;
	
	virtual void NativeDestruct()override;
	
	virtual void InitializeStat(UAbilitySystemComponent* InASC); 
	
protected:
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data); 
	virtual void UpdateStat(const UAbilitySystemComponent* ASC); 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> StatBar; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (ToolTip = "this is the attribute we want to listen for that will be the current value attributem i.e (Health, Mana, Stamina, etc)"))
	FGameplayAttribute TrackedAttribute; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (ToolTip = "this is the attribute we want to listen for that will be the max value of the current attribute we are listening for i.e (Max health, Max Mana, etc)")) 
	FGameplayAttribute TrackedMaxAttribute;
	
	float CurrentValue; 
	float MaxValue;
	
private: 
	
	UPROPERTY()
	UAbilitySystemComponent* ASCRef;
};
