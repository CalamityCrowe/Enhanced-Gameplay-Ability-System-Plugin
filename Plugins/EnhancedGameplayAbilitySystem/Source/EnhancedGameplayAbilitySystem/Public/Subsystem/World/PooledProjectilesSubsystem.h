// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PooledProjectilesSubsystem.generated.h"

/**
 * 
 */


class APooledActorBase;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UPooledProjectilesSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	APooledActorBase* SpawnFromPool(const FTransform& SpawnTransform); 
	
protected: 
	
	APooledActorBase* FindFirstAvailableProjectile();
	
private:
	
	void InitializePool();
	
	UPROPERTY(EditDefaultsOnly, Category = "Pooled Projectiles", meta = (AllowPrivateAccess = true))
	TSubclassOf<APooledActorBase> PooledProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pooled Projectiles", meta = (AllowPrivateAccess = true, UIMin = 0, ClampMin = 0))
	int32 PoolSize = 0;
	
	UPROPERTY()
	TArray<TObjectPtr<APooledActorBase>> PooledProjectiles;
	
};
