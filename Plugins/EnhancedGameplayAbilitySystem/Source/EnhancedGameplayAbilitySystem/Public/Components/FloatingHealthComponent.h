// 

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FloatingHealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UFloatingHealthComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFloatingHealthComponent();

	void SetLookAtTarget(USceneComponent* InTarget); 

protected:
	
	virtual void BeginPlay() override;

	FTimerHandle LookAtTimer; 

	UPROPERTY(EditDefaultsOnly, Category = "UserInterface", meta = (ToolTip = "How often we should be updating the widget to face towards the camera", UIMin = 0.01f, ClampMin = 0.01f))
	float LookAtTime;
	
	UPROPERTY()
	TObjectPtr<USceneComponent> Target;
	
};
