

#include "Weapons/WeaponBase.h"

#include "Subsystem/World/PooledProjectilesSubsystem.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh")); 
	WeaponMesh->SetupAttachment(RootComponent);
	
	// this will be here within the weapon base as we might do smacking someone with a gun, but lower damage 
	HitStart = CreateDefaultSubobject<USceneComponent>(TEXT("HitStart"));
	HitStart->SetupAttachment(WeaponMesh);
	
	HitEnd = CreateDefaultSubobject<USceneComponent>(TEXT("HitEnd"));
	HitEnd->SetupAttachment(WeaponMesh);
}


void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	// we only want to grab this if we have a valid subsystem of this class
	if (UPooledProjectilesSubsystem* TempSub = GetWorld()->GetSubsystem<UPooledProjectilesSubsystem>())
	{
		ProjectilePoolSubsystem = TempSub; 
	}
}


void AWeaponBase::OnWeaponOwnerDestroyed(AActor* DestroyedActor)
{
}



