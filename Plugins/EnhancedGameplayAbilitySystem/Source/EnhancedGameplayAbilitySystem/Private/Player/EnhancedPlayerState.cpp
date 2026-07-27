// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnhancedPlayerState.h"

#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"

// for here we are essentially creating the Ability system component that the player will own in the game. 
// they won't own the ASC themselves, and it means that if our player dies, instead of just creating a new ASC, we can get the new one to use this one as 
//  it will have all the abilities set up already and means we don't need regrant them eseentially.
AEnhancedPlayerState::AEnhancedPlayerState()
{
	ASC = CreateDefaultSubobject<UEnhancedAbilitySystemComponent>(TEXT("Ability System Component")); 
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	AttributeSet = CreateDefaultSubobject<UEnhancedAttributeSet>("Player Attributes");
	
	//. for the purposes of here, we just find the tag associated with death and assign it here. 
	// we set erroring to false, as when this loaded on the initial compile, it will say it can't find it
	// this is due to not being able to find the tag as the asset manager wont have been loaded in at this instant
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"), false); 
}

UAbilitySystemComponent* AEnhancedPlayerState::GetAbilitySystemComponent() const
{
	if (ASC.Get())
	{
		return ASC; 
	}
	return nullptr;
}

UEnhancedAttributeSet* AEnhancedPlayerState::GetAttributeSet() const
{
	if (AttributeSet.Get())
	{
		return AttributeSet;
	}	
	return nullptr;
}

float AEnhancedPlayerState::GetHealth() const
{
	if (AttributeSet.Get())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AEnhancedPlayerState::GetMaxHealth() const
{
	if (AttributeSet.Get())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

bool AEnhancedPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AEnhancedPlayerState::BeginPlay()
{
	Super::BeginPlay(); 
	if (ASC)
	{
		
	}
}

void AEnhancedPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AEnhancedPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float NewHealth = GetHealth();
	if (!IsAlive() && !ASC->HasMatchingGameplayTag(DeadTag))
	{
		// still needs a bit of setup
	}
}
