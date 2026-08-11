// 

#pragma once

#include "CoreMinimal.h"
#include "SocketCollisionParamsObject.generated.h"

// the sole purpose of this class is to define the collision parameters that will be getting sent as payloads in anim notifies for hit scan logic
 
// look in AnimNotifies/AnimNotifyState_HitScanWindow.h 

USTRUCT(BlueprintType)
struct FSocketCollisionParams
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = NAME_None; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CollisionRadius = 0.0f; 
};

UCLASS(BlueprintType)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API USocketCollisionParamsObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSocketCollisionParams Params; 
};
