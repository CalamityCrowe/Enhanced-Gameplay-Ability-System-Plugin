
#include "EditorFiles/PooledProjectileSettings.h"

//plugin 
#include "Actors/ObjectPool/PooledProjectile.h"

UPooledProjectileSettings::UPooledProjectileSettings(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	// this will be the main setting for where it is stored on the left hand side
	CategoryName = "Plugins";
	
	// this is the smaller sub heading within the project tabs
	SectionName = "Projectile Pool"; 
}

#if WITH_EDITOR
FText UPooledProjectileSettings::GetSectionText() const
{
	return NSLOCTEXT("Projectile Pooling Settings","ProjectilePoolSettings","Projectile Pool");
}

FText UPooledProjectileSettings::GetSectionDescription() const
{
	return NSLOCTEXT("Projectile Pooling Settings","Projectile Pooling Settings Description","Everything related to how we want to load in projectiles for ranged weapons");
}
#endif

TSubclassOf<APooledProjectile> UPooledProjectileSettings::GetProjectileActorClass() const
{
	if (ProjectileClass)return ProjectileClass; 
	return nullptr;
}

int32 UPooledProjectileSettings::GetPoolSize() const
{
	return PoolSize;
}
