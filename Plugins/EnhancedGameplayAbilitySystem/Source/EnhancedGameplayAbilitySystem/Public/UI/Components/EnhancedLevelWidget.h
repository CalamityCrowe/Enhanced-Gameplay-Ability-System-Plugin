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
 *  this is a base class for how the widget displaying the current level of the characters is setup.  
 *  
 *  needs a rework to be applied for enemy levels, but this is a case of removing the player state grabbing method and passing in the ASC from the owner
 */

UCLASS(Abstract, Blueprintable)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedLevelWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
	virtual void InitializeAttributeListening(UAbilitySystemComponent* InASC);
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
	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	
};
