// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "EnhancedPlayerState.generated.h"

class UEnhancedAbilitySystemComponent;
class UEnhancedAttributeSet;
/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API AEnhancedPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public: 
	AEnhancedPlayerState(); 
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual UEnhancedAttributeSet* GetAttributeSet() const;
	
	UFUNCTION(BlueprintPure)
	float GetHealth() const;
	
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintPure)
	bool IsAlive() const;
	
protected: 
	UPROPERTY()
	TObjectPtr<UEnhancedAbilitySystemComponent> ASC; 
	
	UPROPERTY()
	TObjectPtr<UEnhancedAttributeSet> AttributeSet;
	
	FGameplayTag DeadTag;
	
	virtual void BeginPlay() override; 
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void HealthChanged(const FOnAttributeChangeData& Data); 
	
	FDelegateHandle OnHealthChangedDelegate;
	
};
