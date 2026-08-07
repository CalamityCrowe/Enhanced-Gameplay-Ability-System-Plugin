// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "EffectActorBase.generated.h"

class UGameplayEffect;
class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEffectActorBase : public AActor
{
	GENERATED_BODY()

public:

	AEffectActorBase();

	
private: 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCapsuleComponent> Collider;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> EffectParticle;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> EffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	FGameplayTag EffectTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true, UIMin = 0.0f, ClampMin = 0.0f))
	float EffectValue; 
	
	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, FActiveGameplayEffectHandle> ActiveEffects;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*  OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor*  OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);
};
