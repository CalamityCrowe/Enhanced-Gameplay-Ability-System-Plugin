// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnhancedGameModeBase.generated.h"

class AEnhancedPlayerCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnhancedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public: 
	AEnhancedGameModeBase(); 	
	
	virtual void PlayerDied(AController* Controller);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode")
	TSubclassOf<AEnhancedPlayerCharacter> PlayerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode", meta = (ClampMin = 0.f, UIMin = 0.0f))
	float RespawnDelay;  
	
	UFUNCTION()
	void RespawnPlayer(AController* Controller); 
};
