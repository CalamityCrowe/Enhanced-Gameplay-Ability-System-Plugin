// 


#include "Player/EnhancedGameModeBase.h"

#include "Characters/Player/EnhancedPlayerCharacter.h"

AEnhancedGameModeBase::AEnhancedGameModeBase(): PlayerClass(nullptr), RespawnDelay(0.0f)
{
	
}

void AEnhancedGameModeBase::PlayerDied(AController* Controller)
{
	Controller->UnPossess(); 
	
	FTimerHandle RespawnTimerHandle;

	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &AEnhancedGameModeBase::RespawnPlayer,
	                                                               Controller); 
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);
	
	// we can do UI stuff here if we wanted to show how long till they respawn
}

void AEnhancedGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnhancedGameModeBase::RespawnPlayer(AController* Controller)
{
	if (Controller->IsPlayerController())
	{
		const AActor* PlayerStart = FindPlayerStart(Controller);
		
		FActorSpawnParameters SpawnParameters; 
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AEnhancedPlayerCharacter* Player = GetWorld()->SpawnActor<AEnhancedPlayerCharacter>(PlayerClass, PlayerStart->GetActorLocation(),PlayerStart->GetActorRotation(),SpawnParameters); 
		Controller->Possess(Player);
		
	}
}
