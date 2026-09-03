// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UI/Formatting/BlueprintNumberFormatting.h"
#include "EnhancedAbilityWidget.generated.h"

class UEnhancedGameplayAbility;
class UImage;
class UOverlay;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedAbilityWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	virtual void NativeConstruct();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Abilities", meta = (Categories = "GameplayAbility") )
	FGameplayTagContainer AbilitiesToFind; 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> AbilityOverlay;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> AbilityIcon; 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> ActiveOverlay;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> ActiveIcon;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> CooldownOverlay;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> CooldownIcon;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;
	
	UPROPERTY(EditDefaultsOnly)
	FBlueprintNumberFormat CooldownTextFormat; 
	
	UPROPERTY()
	FGameplayTag CooldownTag;
	UPROPERTY()
	FGameplayTag ActiveTag;
	
	
	UPROPERTY()
	TObjectPtr<const UEnhancedGameplayAbility> AbilityRef; 
	
	
	void SetAbilityImages(const UEnhancedGameplayAbility& Ability) const;
	
	
private: 
	
	UFUNCTION()
	void CheckForCooldown(FGameplayTag ChangedTag, int32 NewCount);
	
	UFUNCTION()
	void UpdateActiveWidget(FGameplayTag ChangedTag, int32 NewCount) const;
	
	UFUNCTION()
	void UpdateCooldownProgress();
	
	
	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

};
