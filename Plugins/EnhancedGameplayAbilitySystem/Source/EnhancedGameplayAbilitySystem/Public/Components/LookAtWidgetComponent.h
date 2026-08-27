// 

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LookAtWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API ULookAtWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULookAtWidgetComponent();

	virtual void DestroyComponent(bool bPromoteChildren = false) override;
	
	void SetLookAtTarget(USceneComponent* InTarget);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UserInterface", meta = (ToolTip = "How often we want to be updating the widget component to face the camera", UIMin = 0.01f, ClampMin = 0.01f))
	float LookAtInterval;
	UPROPERTY()
	FTimerHandle LookAtTimerHandle;
	UPROPERTY()
	USceneComponent* LookAtTarget; 
	
};
