

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PooledProjectileSettings.generated.h"

class APooledActorBase;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UPooledProjectileSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public: 
	UPooledProjectileSettings(const FObjectInitializer& ObjectInitializer);
protected: 
	UPROPERTY(Config, EditAnywhere)
	TSubclassOf<APooledActorBase> ProjectileClass;
	UPROPERTY(Config, EditAnywhere)
	int32 PoolSize = 0;
	
};
