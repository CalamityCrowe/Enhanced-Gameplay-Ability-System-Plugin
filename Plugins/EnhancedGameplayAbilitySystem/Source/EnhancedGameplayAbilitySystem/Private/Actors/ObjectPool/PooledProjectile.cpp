

#include "Actors/ObjectPool/PooledProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


APooledProjectile::APooledProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement")); 
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 8000.f; 
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->Velocity = FVector(3000.0f, 0.0f, 0.0f); 
	
	Collision->OnComponentHit.AddDynamic(this, &ThisClass::OnComponentHit); 
}

void APooledProjectile::SetProjectileDamage(const FGameplayEffectSpecHandle& InDamageEffect)
{
	EffectSpecHandle = InDamageEffect;
}

void APooledProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void APooledProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor*  OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetInstigator()) return; 
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
	{
		
	}
	SetInUse(false); 
}


