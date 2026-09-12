// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GAS/EnhancedAbilitySet.h"
#include "EnhancedPlayerState.generated.h"

class UEnhancedAbilitySet;
class UEnhancedAbilitySystemComponent;
class UEnhancedAttributeSet;
class UEnhancedCombatAttributesSet;
struct FOnAttributeChangeData; // why this needs to be forward declared now, I have no idea but the compiler bitches about it now (3rd September 2026)
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
	
	virtual UEnhancedCombatAttributesSet* GetCombatAttributeSet() const;
	
	virtual void ResetPlayerHealth();
	
	UFUNCTION(BlueprintPure)
	float GetHealth() const;
	
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentLevel() const;
	
	UFUNCTION(BlueprintPure)
	bool IsAlive() const;
	
protected: 
	UPROPERTY()
	TObjectPtr<UEnhancedAbilitySystemComponent> ASC; 
	
	UPROPERTY()
	TObjectPtr<UEnhancedAttributeSet> AttributeSet;
	
	UPROPERTY()
	TObjectPtr<UEnhancedCombatAttributesSet> CombatAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TObjectPtr<UEnhancedAbilitySet> AbilitySet; 
	
	FGameplayTag DeadTag;
	
	virtual void BeginPlay() override; 
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void HealthChanged(const FOnAttributeChangeData& Data); 
	
	FDelegateHandle OnHealthChangedDelegate;
	
	FAbilitySet_GrantedHandles GrantedAbilityHandles; 

};
