// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ObjectPool/PooledActorBase.h"


APooledActorBase::APooledActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APooledActorBase::BeginPlay()
{
	Super::BeginPlay();
	SetInUse(false); 
}
// we do all the standard can we see it and does the collisions work atm for the actor here
void APooledActorBase::SetInUse(bool InUse)
{
	bInUse = InUse;
	SetActorEnableCollision(bInUse); 
	SetActorHiddenInGame(!bInUse); 
	SetActorTickEnabled(bInUse); 
	GetWorldTimerManager().ClearTimer(ReturnTimer);
	if (bInUse)
	{
		GetWorldTimerManager().SetTimer(ReturnTimer, this, &APooledActorBase::ReturnToPool, TimeToLive, false);
	}
	
}
void APooledActorBase::ReturnToPool()
{
	SetInUse(false);
}


