// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnhancedPlayerController.h"
// engine
#include "EnhancedInputSubsystems.h"

// plugin
#include "Camera/CameraComponent.h"
#include "Components/DamageWidgetComponent.h"
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "Player/EnhancedPlayerState.h"

AEnhancedPlayerController::AEnhancedPlayerController()
{
	
}

void AEnhancedPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// we clear all the inputs on the controller and then assign them with the correct mapping context
	if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSystem->ClearAllMappings(); 
            for (const TPair<TObjectPtr<UInputMappingContext>, int32>& Pair : MappingContexts)
            {
	            if (Pair.Key)
	            {
		            InputSystem->AddMappingContext(Pair.Key, Pair.Value);
	            }
            }
	}
}

void AEnhancedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

// we will grab the player state here for this class. Assuming we aren't subclassing the PS outside of the plugin this is fine
// otherwise we create a matching function for that player state
AEnhancedPlayerState* AEnhancedPlayerController::GetEnhancedPlayerState() const
{
	return CastChecked<AEnhancedPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}


UEnhancedAbilitySystemComponent* AEnhancedPlayerController::GetEnhancedAbilitySystemComponent() const
{
	 if(AEnhancedPlayerState* PS = GetEnhancedPlayerState())
	 {
			return CastChecked<UEnhancedAbilitySystemComponent>(PS->GetAbilitySystemComponent()); 
	 }
	return nullptr; 
}

void AEnhancedPlayerController::ShowDamageNumber(const float InDamage, AActor* TargetActor)
{
	if (!TargetActor || !DamageComponentClass) return; 
	
	UDamageWidgetComponent* DamageWidgetComponent = NewObject<UDamageWidgetComponent>(TargetActor, DamageComponentClass); // presuming the widget component has been subclassed in blueprint
	DamageWidgetComponent->RegisterComponent();
	DamageWidgetComponent->AttachToComponent(TargetActor->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform); 
	DamageWidgetComponent->SetDamageText(InDamage);
	DamageWidgetComponent->SetLookAtTarget(GetPawn()->FindComponentByClass<UCameraComponent>()); 
	
}

// We override the processing of the inputs to handle the ability inputs here
void AEnhancedPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(UEnhancedAbilitySystemComponent* ASC = GetEnhancedAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused); 
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

// this was in a old project, no idea why I have it, but there is a reason apparently so \_0_/
void AEnhancedPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}
