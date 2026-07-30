// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/AICharacterBase.h"

#include "AI/EnemyControllerBase.h"
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"

AAICharacterBase::AAICharacterBase()
{
	HardRefASC = CreateDefaultSubobject<UEnhancedAbilitySystemComponent>(TEXT("ASC")); 
	ASC = HardRefASC; 
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Full); 
	
	HardRefAttributeSet = CreateDefaultSubobject<UEnhancedAttributeSet>(TEXT("AttributeSet")); 
	AttributeSet = HardRefAttributeSet;
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (!ASC.IsValid() || !AbilitySet)return; 
	
	ASC->InitAbilityActorInfo(this, this); 
	
	AbilitySet->GiveToAbilitySystem(ASC.Get(), &GrantedAbilityHandles, this);
	
	ASC->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AAICharacterBase::HealthUpdated); 
	
}

void AAICharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AAICharacterBase::HealthUpdated(const FOnAttributeChangeData& Data)
{
	float NewHealth = Data.NewValue; 
	
	// if we are doing UI stuff here to signal UI, we could pass the health here
	
	if (!IsAlive()|| !ASC->HasMatchingGameplayTag(DeadTag))
	{
		Die(); 
	}
}

void AAICharacterBase::Die()
{
	if (AEnemyControllerBase* AIC = Cast<AEnemyControllerBase>(GetController()))
	{
		AIC->StopBehaviourTree(); 
	}
	
	Super::Die();
}

void AAICharacterBase::FinishDying()
{
	// if we are doing object pooling, we could have them go back to a pool here instead of destroying them in the base class
	Super::FinishDying();
}


