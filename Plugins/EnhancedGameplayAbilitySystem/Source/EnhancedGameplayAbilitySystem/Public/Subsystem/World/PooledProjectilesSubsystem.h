// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PooledProjectilesSubsystem.generated.h"

/**
 *  since there will be some kind of projectiles in every level, we use this subsystem as the projectile pool manager
 *  
 *  this means that we don't need to worry about the pool being invalid when we load the world.
 */
class APooledProjectile;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UPooledProjectilesSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	// this is the function that we will be using to handle getting from the pool
	APooledProjectile* SpawnFromPool(const FTransform& SpawnTransform); 
	
protected: 
	
	APooledProjectile* FindFirstAvailableProjectile();
	
private:
	
	void InitializePool();
	
	UPROPERTY(EditDefaultsOnly, Category = "Pooled Projectiles", meta = (AllowPrivateAccess = true))
	TSubclassOf<APooledProjectile> PooledProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pooled Projectiles", meta = (AllowPrivateAccess = true, UIMin = 0, ClampMin = 0))
	int32 PoolSize = 0;
	
	UPROPERTY()
	TArray<TObjectPtr<APooledProjectile>> PooledProjectiles;
	
	
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
};
