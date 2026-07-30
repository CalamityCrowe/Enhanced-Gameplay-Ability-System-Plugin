#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledActorBase.generated.h"

UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API APooledActorBase : public AActor
{
	GENERATED_BODY()

public:
	APooledActorBase();

	void SetTimeToLive(float InTimeToLive){TimeToLive = InTimeToLive;}
	virtual void SetInUse(bool InUse); 
	
	bool IsInUse() const {return bInUse; }
protected:
	
	virtual void BeginPlay() override;
	
	void ReturnToPool(); 

private: 
	bool bInUse; 
	float TimeToLive;
	
	FTimerHandle ReturnTimer; 
};
