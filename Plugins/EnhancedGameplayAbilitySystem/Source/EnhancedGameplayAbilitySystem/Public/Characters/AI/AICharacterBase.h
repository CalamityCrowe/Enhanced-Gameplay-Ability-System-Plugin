// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "AICharacterBase.generated.h"

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AAICharacterBase : public ACharacterBase
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
private: 
	UPROPERTY()
	UEnhancedAttributeSet* HardRefAttributeSet; 
	
	UPROPERTY()
	UEnhancedAbilitySystemComponent* HardRefASC; 
};
