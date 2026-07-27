// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "EnhancedAbilitySet.generated.h"

class UEnhancedGameplayAbility; 
class UEnhancedAbilitySystemComponent; 

// this will be used by the AbilitySet to define abilities and the input tags to grant
USTRUCT(Blueprintable)
struct FAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UEnhancedGameplayAbility> Ability;
	
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel = 1; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (Category = "InputTag"))
	FGameplayTag AbilityInputTag; 
};

USTRUCT(BlueprintType)
struct FAbilitySet_GrantedHandles
{
	GENERATED_BODY()
public: 
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	
public: 
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};



UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UEnhancedAbilitySet : public UDataAsset
{
	GENERATED_BODY()
public: 
	void GiveToAbilitySystem(UEnhancedAbilitySystemComponent* InASC, FAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = "Ability"))
	TArray<FAbilitySet_GameplayAbility> GrantedAbilities; 
};
