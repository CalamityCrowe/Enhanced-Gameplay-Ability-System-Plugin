// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "GAS/EnhancedAbilitySet.h"
#include  "States/HitDirections.h"
#include "CharacterBase.generated.h"

class UGameplayEffect;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHitReactDelegate, EHitReactDirection, Direction); 


class UEnhancedAbilitySystemComponent; 
class UEnhancedAttributeSet; 

UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACharacterBase();
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Character", meta = (ToolTip = "This will be used to bind a delegate within the animation blueprint on how we should handle hit reactions. We don't really need to use this if we dont have hit reacts, but this will fire on all damage events either way"))
	FCharacterHitReactDelegate CharacterHitReactDelegate;
	

	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	EHitReactDirection GetHitReactDirection(const FVector ImpactPoint) const;
	
	void PlayHitReact(FGameplayTag HitDirection); 
	
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle>AbilitiesToRemove); 
	
	float GetHealth() const;

	int32 GetCurrentLevel() const; 
	
	bool IsAlive() const; 
	
	virtual void Die(); 
	
	UFUNCTION(BlueprintCallable)
	virtual void FinishDying(); 

	virtual void LevelUp();
	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintCallable, Category= "GAS|Event Functions")
	void SendGameplayEventToSelf(const FGameplayEventData& EventData);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TWeakObjectPtr<UEnhancedAbilitySystemComponent> ASC; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TWeakObjectPtr<UEnhancedAttributeSet> AttributeSet; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input System|Abilities", meta = (ToolTip = "This is a data asset that contains all the default abilities that the character will have.\nIf we have a melee attack by default, we would assign it here in this data asset along with "))
	TObjectPtr<UEnhancedAbilitySet> AbilitySet; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Death", meta = (ToolTip = "This is the death montage that will be played when the 'DeadTag' has been added to the ASC. This is why the 'FinishDying' function is blueprint Callable as it is part of a anim notify to signal that it is finished"))
	TObjectPtr<UAnimMontage> DeathMontage; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Death", meta = (ToolTip = "This tag is what the ASC will be listening for to signal that the character is actually dead"))
	FGameplayTag DeadTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Level")
	TSubclassOf<UGameplayEffect> NextLevelEffect; 
	
	
	FAbilitySet_GrantedHandles GrantedAbilityHandles; 
	
	FGameplayTag HitDirectionFront; 
	FGameplayTag HitDirectionBack; 
	FGameplayTag HitDirectionLeft; 
	FGameplayTag HitDirectionRight; 
		
	
	
};
