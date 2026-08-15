// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/RootMotionSource.h"
#include "GAS/Ability/EnhancedEventAbility.h"
#include "MovementAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UMovementAbility : public UEnhancedEventAbility
{
	GENERATED_BODY()
public:
	UMovementAbility(); 
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta = (ToolTip = "The amount of force we want to apply for the root motion", UIMin = 0.0f, ClampMin = 0.0f))
	float Strength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta = (ToolTip = "How long we want to be applying the force to the owning character", UIMin = 0.0f, ClampMin = 0.0f))
	float Duration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta = (ToolTip = "Do we want to add to the current velocity or override it?"))
	bool bIsAdditive;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta = (ToolTip = "Determines how much force it should be applying over a scale. \n\n 0 means none, 1 the default amount"))
	TObjectPtr<UCurveFloat> StrengthOverTimeCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta = (ToolTip = "We determine what we want the characters velocity to do on finish"))
	ERootMotionFinishVelocityMode VelocityOnFinishMode; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta  =(ToolTip = "Do we want the character to be affected by gravity?"))
	bool bEnableGravity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Movement Properties", meta = (ToolTip = "if we aren't moving, do we want the default move direction to be towards where we are looking or the actors forward"))
	bool bMovetoCameraDirection; 
	
	virtual void MontageStarted_Implementation() override;

	virtual void OnEventTriggered_Implementation(FGameplayEventData Payload) override;
	
	UFUNCTION()
	void OnDashFinish();
	
	float GetMaxSpeed() const;
	
	FVector GetMoveDirection() const; 
	
};
