// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/ThirdPersonPlayerCharacter.h"

// plugin
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"
#include "Player/EnhancedPlayerState.h"


// engine
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AThirdPersonPlayerCharacter::AThirdPersonPlayerCharacter()
{
	// this is the general setup that we will be using for the third person character and will be what the first person character will inherit from
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetWorldLocation(FVector(0,0,8.492f)); 
	CameraBoom->SocketOffset = FVector(0,40.f, 60.f); 
	CameraBoom->TargetArmLength = 400.f; 
	CameraBoom->bUsePawnControlRotation = true; 
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); 
	Camera->SetupAttachment(CameraBoom, CameraBoom->SocketName);
	
	bUseControllerRotationYaw = false;
}

void AThirdPersonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThirdPersonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AThirdPersonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AEnhancedPlayerState* PS = Cast<AEnhancedPlayerState>(GetPlayerState()); 
	check(PS); 
	
	ASC = Cast<UEnhancedAbilitySystemComponent>(PS->GetAbilitySystemComponent()); 
	ASC->InitAbilityActorInfo(this, this); 
	
	AttributeSet = PS->GetAttributeSet();  
	
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(ASC.Get(),&GrantedAbilityHandles, this); 
	}
	
	SendAbilitiesChangedEvent(); 
}

// in this instance,
void AThirdPersonPlayerCharacter::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData; 
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Abilities.Changed")); 
	EventData.Instigator = this;
	EventData.Target = this; 
	
	SendGameplayEventToSelf(EventData); 
}

void AThirdPersonPlayerCharacter::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	ASC->AbilityInputTagPressed(InputTag);
}

void AThirdPersonPlayerCharacter::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	ASC->AbilityInputTagReleased(InputTag);
}

void AThirdPersonPlayerCharacter::Die()
{
	Super::Die();
	// when the player dies, we want to grab the controller and disable the input for them
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC); 
	}
}

void AThirdPersonPlayerCharacter::FinishDying()
{
	Super::FinishDying();
}

// gets the relevant movement direction from the 
void AThirdPersonPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementAxis = Value.Get<FVector2D>(); 
	FRotator ControlRotation = GetControlRotation();
	
	FVector XWorldVector = UKismetMathLibrary::GetRightVector(FRotator(0.f,ControlRotation.Yaw,ControlRotation.Roll)); 
	FVector YWorldVector = UKismetMathLibrary::GetForwardVector(FRotator(0.f, ControlRotation.Yaw,0.f));
	
	AddMovementInput(XWorldVector, MovementAxis.X, false); 
	AddMovementInput(YWorldVector, MovementAxis.Y, false); 
}

// the standard look input you would use
void AThirdPersonPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxis.X); 
	AddControllerYawInput(LookAxis.Y);
}

