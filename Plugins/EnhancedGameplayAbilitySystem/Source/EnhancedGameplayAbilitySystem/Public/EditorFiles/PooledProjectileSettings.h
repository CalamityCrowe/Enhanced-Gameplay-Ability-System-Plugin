

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
	
#if WITH_EDITOR
	virtual FText  GetSectionText() const override; 
	virtual FText GetSectionDescription() const override;
#endif
	
	UFUNCTION(BlueprintPure, Category= "Pooled Projectiles")
	TSubclassOf<APooledActorBase> GetProjectileActorClass() const;
	UFUNCTION(BlueprintPure, Category = "Pooled Projectiles")
	int32 GetPoolSize() const; 
protected: 
	UPROPERTY(Config, EditAnywhere)
	TSubclassOf<APooledActorBase> ProjectileClass;
	UPROPERTY(Config, EditAnywhere)
	int32 PoolSize = 0;
	
	
};
