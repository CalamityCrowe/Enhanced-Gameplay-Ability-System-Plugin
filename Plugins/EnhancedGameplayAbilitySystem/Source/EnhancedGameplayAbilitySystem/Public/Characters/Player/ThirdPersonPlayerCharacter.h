// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "ThirdPersonPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UEnhancedInputConfig;

// we dont want to make this visible in the editor
UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AThirdPersonPlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonPlayerCharacter();

	
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
