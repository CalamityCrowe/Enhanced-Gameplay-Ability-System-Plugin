// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/World/PooledProjectilesSubsystem.h"

// plugin
#include "Actors/ObjectPool/PooledActorBase.h"
#include "Actors/ObjectPool/PooledProjectile.h"
#include "EditorFiles/PooledProjectileSettings.h"


// when we initialise the pool, we reset the previous pool and then pool the relevant information from the developer settings  
void UPooledProjectilesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PooledProjectiles.Reset(); 
	
	if (const UPooledProjectileSettings* Settings = GetDefault<UPooledProjectileSettings>())
	{
		PooledProjectileClass = Settings->GetProjectileActorClass(); 
		PoolSize = Settings->GetPoolSize();
		InitializePool(); 
	}
}

void UPooledProjectilesSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

// when we spawn from the pool, we will grab the first available projectile to be returned. 
// At this point we don't set in use as there will still be properties needing to be set (Damage, Gameplay effects)
APooledProjectile* UPooledProjectilesSubsystem::SpawnFromPool(const FTransform& SpawnTransform)
{
	// I wont set in use here, as we might need to do something specific with the projectile when we are using it
	if (APooledProjectile* AvailableActor = FindFirstAvailableProjectile())
	{
		AvailableActor->SetActorTransform(SpawnTransform);
		//AvailableActor->SetInUse(true);
		return AvailableActor;
	}
	return nullptr;
}

// for finding the first available actor, we return the first actor not in use
APooledProjectile* UPooledProjectilesSubsystem::FindFirstAvailableProjectile()
{
	for (APooledProjectile* PooledActor : PooledProjectiles)
	{
		if (PooledActor->IsInUse() == false)
		{
			return PooledActor; 
		}
	}
	return nullptr;
}


void UPooledProjectilesSubsystem::InitializePool()
{
	if (!PooledProjectileClass)
	{
#if WITH_EDITOR
	UE_LOG(LogTemp, Error, TEXT("Pooled actor class is null in the pooled projectile subsystem"))
#endif
		return; 
	}
	for (int i = 0; i < PoolSize; ++i)
	{
		FActorSpawnParameters SpawnParams;
		if (APooledProjectile* NewActor = GetWorld()->SpawnActor<APooledProjectile>(PooledProjectileClass, FVector::ZeroVector,FRotator::ZeroRotator, SpawnParams))
		{
			NewActor->SetInUse(false); 
			PooledProjectiles.AddUnique(NewActor);
		}
	}
}

// we only run this subsystem if it is in the game or play-in-editor
bool UPooledProjectilesSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}
