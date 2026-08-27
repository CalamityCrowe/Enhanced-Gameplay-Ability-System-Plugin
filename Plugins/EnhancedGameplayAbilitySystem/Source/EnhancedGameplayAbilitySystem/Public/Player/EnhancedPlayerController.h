// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedPlayerController.generated.h"


class UDamageWidgetComponent;
class AEnhancedPlayerState;
class UEnhancedAbilitySystemComponent;
class UInputMappingContext;
/**
 * this is a very abstracted class for this plugin, as a lot of this will require the player state that has been built within an actual project. 
 * 
 * All the functionality will be included as virtual functions with comments saying what needs to be added to make it functional
 * 
 * This controller specifically will be what processes the inputs for abilities specifically, and makes sure that they are not getting triggered multiple times if they are only to fire once
 */



UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnhancedPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AEnhancedPlayerController();
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintPure, Category = "Input System|Player State")
	virtual AEnhancedPlayerState* GetEnhancedPlayerState() const;
	
	UFUNCTION(BlueprintPure, Category = "Input System|Ability System")
	virtual UEnhancedAbilitySystemComponent* GetEnhancedAbilitySystemComponent() const;
	
	virtual void ShowDamageNumber(const float InDamage, AActor* TargetActor); 
	
	
protected:
	
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	virtual void OnRep_PlayerState() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input System|Inputs", meta = (ToolTip = "we add all the mapping context that we will be using in game here\nThe Key of the map is a Inputmapping context and the value is the priority for the mapping context\nwe would do this to specific inputs seperate, like ability based ones, pause or generic character movements"))
	TMap<TObjectPtr<UInputMappingContext>, int32> MappingContexts;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDamageWidgetComponent> DamageComponentClass;
	
	
	// if we are doing paused inputs, etc. then this is where we would handle that here by setting up all the relevant HUD classes to retrieve along with the behaviour of what we want to happen
	// so if we hard pause the game like bioshock, it takes 2, etc or if we do a dark souls like pause where it overlays the screen and just disables attacks on the player whilst everything else can attack
};
