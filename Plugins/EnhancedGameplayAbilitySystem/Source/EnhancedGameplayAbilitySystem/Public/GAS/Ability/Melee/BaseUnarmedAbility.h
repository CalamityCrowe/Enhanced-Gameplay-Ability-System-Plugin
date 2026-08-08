// 

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/EnhancedGameplayAbility.h"
#include "BaseUnarmedAbility.generated.h"

/**
 * this is a reference on how to go about implementing a basic hit scan, ideally we wouldn't do the actual hit scan in the 
 * ability but rather we would signal this to a weapon manager. this is more of a reference point on how to do this without one in place
 */
UCLASS(Abstract, HideDropDown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UBaseUnarmedAbility : public UEnhancedGameplayAbility
{
	GENERATED_BODY()
public: 
	UBaseUnarmedAbility(); 
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void MontageStarted(){}; // left blank in this class, but we can override this in later classes to add additional functionality like combos being recieved
protected: 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Animation")
	TObjectPtr<UAnimMontage> MontageToPlay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Effects", meta = (ToolTip = "The effect that will be getting applied to the hit targets"))
	TSubclassOf<UGameplayEffect> EffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Effects", meta = (ToolTip = "The tag that will be getting used to apply to a 'set caller by magnitude' if the effect has one"))
	FGameplayTag EffectMagnitudeTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Effects", meta = (ToolTip = "The value for the effects magnitude if it is applying via 'set caller by magnitude'"))
	float EffectMagnitude;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Events", meta = (ToolTip = "This is the tag that we will be listening for getting sent from the montage to signal that the hit scan has started"))
	FGameplayTag HitScanStartTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Events", meta = (ToolTip = "This is the tag that we will be listening for getting sent from the montage to signal that the hit scan has ended"))
	FGameplayTag HitScanEndTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Hit Scan Properties", meta = (ToolTip = "This is the object types we want to hit (Pawn, World Static, etc)"))
	TArray<TEnumAsByte<EObjectTypeQuery>> HitableObjectTypes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Hit Scan Properties", meta = (ToolTip = "This is how often we want the hit scan to be performed. \n\n for 60 times a seconds, do 1/60 and the higher this is the more resources it takes",UIMin = 0.0f, ClampMin = 0.0f))
	float ScanInterval;
	
	UPROPERTY()
	FTimerHandle HitScanTimer; 
	
	FName SocketName;
	float CollisionRadius; 
	
	UPROPERTY()
	TArray<AActor*> HitActors; 
	
	UFUNCTION()
	virtual void OnMontageCompleted(); 
	
	UFUNCTION()
	virtual void OnMontageCancelled();
	
	UFUNCTION()
	virtual void OnMontageBlendOut(); 
	
	UFUNCTION()
	virtual void HitScanStarted(FGameplayEventData Payload); 
	
	UFUNCTION()
	virtual void HitScanEnded(FGameplayEventData Payload);
	
	UFUNCTION()
	virtual void HitScan(); 
	
};
