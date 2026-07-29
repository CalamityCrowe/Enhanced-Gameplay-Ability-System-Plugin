// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/ThirdPersonPlayerCharacter.h"

// plugin
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"
#include "Player/EnhancedPlayerState.h"


// engine
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "EditorFiles/EnhancedGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/GASEnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AThirdPersonPlayerCharacter::AThirdPersonPlayerCharacter()
{
	// this is the general setup that we will be using for the third person character and will be what the first person character will inherit from
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetWorldLocation(FVector(0,0,8.492f)); 
	CameraBoom->SocketOffset = FVector(0,40.f, 60.f); 
	CameraBoom->TargetArmLength = 250.f; 
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
	
	UGASEnhancedInputComponent* GASInputComponent = Cast<UGASEnhancedInputComponent>(PlayerInputComponent);
	check(GASInputComponent);
	
	const FEnhancedGameplayTags& GameplayTags = FEnhancedGameplayTags::Get();
	TArray<uint32> BindHandles; 
	
	// from the input config we will grab all the ability specific inputs here, then bind the pressed and release actions
	// how the ability behaves is handled in the background by the ASC and will be based on how the abilities input mode
	GASInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::InputAbilityInputTagPressed, &ThisClass::InputAbilityInputTagReleased, BindHandles); 
	
	// for the native inputs, this is what will be defined by the player specifically. Logically this doesn't make sense to be an ability as it's generic functionality like player movement
	// but this does mean we can block these using the tag "NativeInput" in the specific abilities
	GASInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	GASInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Aim, ETriggerEvent::Triggered, this, &ThisClass::Look);
	GASInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Jump, ETriggerEvent::Started, this, &ThisClass::Jump); 
	GASInputComponent->BindNativeAction(InputConfig, GameplayTags.Input_Jump, ETriggerEvent::Canceled, this, &ThisClass::StopJumping); 
}

// in the possessed by functionality, we can grab the player state and then assign the ASC correctly here for the player to have a reference to along with the attributes
void AThirdPersonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AEnhancedPlayerState* PS = Cast<AEnhancedPlayerState>(GetPlayerState()); 
	check(PS); 
	
	ASC = Cast<UEnhancedAbilitySystemComponent>(PS->GetAbilitySystemComponent()); 
	ASC->InitAbilityActorInfo(this, this); 
	
	AttributeSet = PS->GetAttributeSet();  
	
	// in the ability set, we get all the 
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
	AddControllerPitchInput(LookAxis.Y);
}

