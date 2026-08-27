// 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_UCLASS_BODY()
public: 
	virtual void NativeConstruct() override;
	
	void SetDamageNumberText(float InDamage) const; 
	
	
protected: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Number", meta = (Bindwidget))
	TObjectPtr<UTextBlock> DamageText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Transient,meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnimation; 
	
};
