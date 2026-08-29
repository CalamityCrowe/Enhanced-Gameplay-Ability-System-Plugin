// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Formatting/BlueprintNumberFormatting.h"
#include "GameplayEffectTypes.h"
#include "EnhancedLevelWidget.generated.h"

class UAbilitySystemComponent;
class UTextBlock;
class UImage;
class USizeBox;
/**
 * 
 */

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedLevelWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
protected: 
	
	virtual void OnAttributeChanged(const FOnAttributeChangeData& Data); 
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> WidgetSize;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> BackgroundIcon;
	UPROPERTY(EditDefaultsOnly,	meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;
	
	UPROPERTY(EditDefaultsOnly)
	FBlueprintNumberFormat LevelTextFormat; 
	
private: 
	FDelegateHandle LevelChangedHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* CachedASC;
	
};
