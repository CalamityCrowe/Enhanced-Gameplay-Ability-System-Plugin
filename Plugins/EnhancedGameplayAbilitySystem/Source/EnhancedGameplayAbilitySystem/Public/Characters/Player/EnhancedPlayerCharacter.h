// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "EnhancedPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UEnhancedInputConfig;

// since this is a reference to how we might want to implement a player character, this one is a setup for third person characters instead
// if we were doing a first person, we would give another mesh here that hides itself to other players and is visible to the owning player 
// so the hands are in the correct place. would look at the first-person shooter sample project for a reference

// we dont want to make this visible in the editor
UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnhancedPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnhancedPlayerCharacter();

	
	virtual void Die() override;
	virtual void FinishDying() override;
	
protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PossessedBy(AController* NewController) override;
	void SendAbilitiesChangedEvent(); 
	
	
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input System|Inputs", meta = (ToolTip = "This is where all the inputs will be stored that will be bound to the character"))
	TObjectPtr<UEnhancedInputConfig> InputConfig; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;
	
private: 
	
	UFUNCTION()
	virtual void Move(const FInputActionValue& Value); 
	UFUNCTION()
	virtual void Look(const FInputActionValue& Value);
	
	
	
};
