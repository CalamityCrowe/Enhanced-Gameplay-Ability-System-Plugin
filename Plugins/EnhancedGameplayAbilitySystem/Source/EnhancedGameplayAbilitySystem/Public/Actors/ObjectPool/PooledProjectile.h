// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "PooledActorBase.h"
#include "PooledProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

// STILL NEEDS TESTED TO SEE IF IT WORKS

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API APooledProjectile : public APooledActorBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APooledProjectile();
	
	void SetProjectileDamage(const FGameplayEffectSpecHandle& InDamageEffect); 
	
	virtual void SetInUse(bool InUse) override;
	
protected:
	virtual void BeginPlay() override;
	

	
private: 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> Collision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement; 
	
	UPROPERTY()
	FGameplayEffectSpecHandle EffectSpecHandle; 
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor*  OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	
	FVector DefaultVelocity = FVector::ZeroVector;
	
};
