// 

#pragma once

#include "CoreMinimal.h"
#include "Components/LookAtWidgetComponent.h"
#include "DamageWidgetComponent.generated.h"


class UDamageNumberWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UDamageWidgetComponent : public ULookAtWidgetComponent
{
	GENERATED_BODY()

public:
	UDamageWidgetComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetDamageText(const float InDamage);

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "User Interface", meta = (ToolTip = "How long the component is allowed to live", UIMin = 0.1f, ClampMin = 0.1f))
	float TimeToLive; 
	
	
	UPROPERTY(EditDefaultsOnly, Category = "User Interface", meta = (ToolTip = "The minimum value for the movement speed"))
	float MinMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "User Interface", meta = (ToolTip = "The maximum value for the movement speed"))
	float MaxMoveSpeed;
	
	UPROPERTY()
	float MoveSpeed;
	
	// renaming this later
	FTimerHandle LiveTimer;
	
	FTimerHandle LookAtTimer;
	
	FVector MoveDirection;
	
	UPROPERTY()
	USceneComponent* LookAtComponent; 
	
	
	void MoveWidget(); 
	
};
