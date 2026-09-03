

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PooledProjectileSettings.generated.h"

class APooledProjectile;

/**
 *  This is the settings that will be getting used to define what projectiles are getting pooled in the game
 *  
 *  These are found under project settings. Think of this as a helper class for the world subsystem to use to setup correctly 
 */
UCLASS(Config=Game, DefaultConfig)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UPooledProjectileSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public: 
	UPooledProjectileSettings(const FObjectInitializer& ObjectInitializer);
	
#if WITH_EDITOR
	virtual FText  GetSectionText() const override; 
	virtual FText GetSectionDescription() const override;
#endif
	
	UFUNCTION(BlueprintPure, Category= "Pooled Projectiles")
	TSubclassOf<APooledProjectile> GetProjectileActorClass() const;
	UFUNCTION(BlueprintPure, Category = "Pooled Projectiles")
	int32 GetPoolSize() const; 
protected: 
	UPROPERTY(Config, EditAnywhere)
	TSubclassOf<APooledProjectile> ProjectileClass;
	UPROPERTY(Config, EditAnywhere)
	int32 PoolSize = 0;
	
	
};
