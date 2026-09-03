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
// if we are implementing particles and such in a later class, this is the one that we override for that
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
// when it says return, it essentially means turning everything off
void APooledActorBase::ReturnToPool()
{
	SetInUse(false);
}


