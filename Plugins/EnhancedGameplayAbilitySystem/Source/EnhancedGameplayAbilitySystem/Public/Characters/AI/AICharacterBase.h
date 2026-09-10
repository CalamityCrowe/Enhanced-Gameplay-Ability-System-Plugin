// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/EnemyAIInterface.h"
#include "AICharacterBase.generated.h"

class UFloatingHealthComponent;

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AAICharacterBase : public ACharacterBase, public IEnemyAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void HealthUpdated(const FOnAttributeChangeData& Data); 
	
	virtual void Die() override;
	
	virtual void FinishDying() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FAIMovementProperties AIMovementProperties;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FAICombatParameters AICombatParameters;
	
private: 
	UPROPERTY()
	UEnhancedAttributeSet* HardRefAttributeSet; 
	
	UPROPERTY()
	UEnhancedAbilitySystemComponent* HardRefASC;
	
	UPROPERTY()
	UEnhancedCombatAttributesSet* HardRefCombatAttributeSet; 
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UFloatingHealthComponent> FloatingHealthComponentClass;
	
	virtual void SetMovementSpeed_Implementation(EMovementSpeed& SpeedMode, float& MovementSpeed) override;
	virtual void GetIdealRange_Implementation(float& OutAttackRange, float& OutDefendRange) const override;
	
};
