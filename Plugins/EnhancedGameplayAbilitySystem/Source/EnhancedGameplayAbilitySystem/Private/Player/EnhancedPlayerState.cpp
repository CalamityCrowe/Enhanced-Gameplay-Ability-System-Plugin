// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EnhancedPlayerState.h"

#include "Characters/Player/EnhancedPlayerCharacter.h"
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"
#include "GAS/Attributes/EnhancedCombatAttributesSet.h"

// for here we are essentially creating the Ability system component that the player will own in the game. 
// they won't own the ASC themselves, and it means that if our player dies, instead of just creating a new ASC, we can get the new player to use this one as 
//  it will have all the abilities set up already and means we don't need regrant them.
AEnhancedPlayerState::AEnhancedPlayerState()
{
	ASC = CreateDefaultSubobject<UEnhancedAbilitySystemComponent>(TEXT("Ability System Component")); 
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	
	AttributeSet = CreateDefaultSubobject<UEnhancedAttributeSet>("Player Attributes");
	CombatAttributeSet = CreateDefaultSubobject<UEnhancedCombatAttributesSet>("Combat Attributes"); 
	//. for the purposes of here, we just find the tag associated with death and assign it. 
	// we set erroring to false, as when this loaded on the initial compile, it will say it can't find it
	// this is due to not being able to find the tag as the asset manager wont have been loaded in at this point
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

UEnhancedCombatAttributesSet* AEnhancedPlayerState::GetCombatAttributeSet() const
{
	if (CombatAttributeSet.Get())
	{
		return CombatAttributeSet;
	}
	return nullptr;
}

void AEnhancedPlayerState::ResetPlayerHealth()
{
	ASC->RemoveLooseGameplayTag(DeadTag);
	
	UGameplayEffect* GERespawn = NewObject<UGameplayEffect>(GetTransientPackage(), FName("Respawn"));
	GERespawn->DurationPolicy = EGameplayEffectDurationType::Instant; 
	
	const int32 Index = GERespawn->Modifiers.Num(); 
	GERespawn->Modifiers.SetNum(Index + 1); // since we are only reseting health here
	
	FGameplayModifierInfo& InfoHealth = GERespawn->Modifiers[Index];
	InfoHealth.ModifierMagnitude = FScalableFloat(GetMaxHealth()); 
	InfoHealth.ModifierOp = EGameplayModOp::Override; 
	InfoHealth.Attribute = UEnhancedAttributeSet::GetHealthAttribute(); 
	
	ASC->ApplyGameplayEffectToSelf(GERespawn, GetCurrentLevel(),ASC->MakeEffectContext()); 
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

float AEnhancedPlayerState::GetCurrentLevel() const
{
	if (AttributeSet.Get())
		return AttributeSet->GetLevel(); 
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
		OnHealthChangedDelegate = ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged); 
	}
	
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(ASC.Get(),&GrantedAbilityHandles, this); 
	}
}

void AEnhancedPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (FGameplayAbilitySpecHandle& AbilityHandle: GrantedAbilityHandles.GrantedAbilitySpecHandles)
	{
		ASC->ClearAbility(AbilityHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void AEnhancedPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float NewHealth = GetHealth();
	if (!IsAlive() && !ASC->HasMatchingGameplayTag(DeadTag))
	{
		if (AEnhancedPlayerCharacter* Player = Cast<AEnhancedPlayerCharacter>(GetPawn()))
		{
			Player->Die();
		}
	}
}
