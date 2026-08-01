// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UPooledProjectilesSubsystem;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();
	
	
protected:
	UFUNCTION()
	void OnWeaponOwnerDestroyed(AActor* DestroyedActor); 
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> HitStart;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> HitEnd;
	
	UPROPERTY()
	UPooledProjectilesSubsystem* ProjectilePoolSubsystem; 
};
