// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"

// engine
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// enhanced ability Plugin
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Attributes/EnhancedAttributeSet.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f); 
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"), false);
	
	HitDirectionFront = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"), false); 
	HitDirectionBack = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"), false); 
	HitDirectionLeft = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"), false); 
	HitDirectionRight = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"), false); 
	
	GetMesh()->bRenderCustomDepth = true; 
	GetMesh()->CustomDepthStencilValue = 1;
	
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// this will run whenever a character gets a controller possession. This can be on either the player or any kind of AI, and will probably be expanded to have more specific contexts
void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (ASC.Get())
	{
		ASC->InitAbilityActorInfo(this, this); 
	}
}

// this will ony run if it is the player as far as I am aware
void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (ASC.Get())
	{
		ASC->InitAbilityActorInfo(this, this); 
	}
}

// this is a generic event sender to be used to send event data to the player
void ACharacterBase::SendGameplayEventToSelf(const FGameplayEventData& EventData)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData); 
}

// we are checking the ASC is valid and if so we return that, otherwise we return null
UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	if (ASC.Get())
	{
		return ASC.Get();
	}	
	return nullptr; 
}

// The method to decide what hit direction to be done using the dot product against the impact location 
EHitReactDirection ACharacterBase::GetHitReactDirection(const FVector ImpactPoint) const
{
	// we cache this value to reduce repetitive calls back to the actor location
	FVector ActorLocation = GetActorLocation();
	
	// the point plane distance is essentially offsetting the point against the actors location, then performing the dot product
	// positive in the front, negative in the back
	float ForwardDistance = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());
	// positive in the right, negative in the left
	float RightDistance = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	
	// we then check if it greater in the forward direction or the right direction. and then check if the dot value was in the negative or positive to get the final value
	if (abs(ForwardDistance) >= abs(RightDistance))
	{
		return ForwardDistance >= 0? EHitReactDirection::Front: EHitReactDirection::Back;
	}
	else
	{
		return RightDistance >= 0? EHitReactDirection::Right: EHitReactDirection::Left;
	}
}


void ACharacterBase::PlayHitReact(FGameplayTag HitDirection)
{
	if (IsAlive())
	{
		if (HitDirection == HitDirectionFront)
		{
			CharacterHitReactDelegate.Broadcast(EHitReactDirection::Front);
		}
		else if (HitDirection == HitDirectionBack)
		{
			CharacterHitReactDelegate.Broadcast(EHitReactDirection::Back);
		}
		else if (HitDirection == HitDirectionLeft)
		{
			CharacterHitReactDelegate.Broadcast(EHitReactDirection::Left);
		}
		else
		{
			CharacterHitReactDelegate.Broadcast(EHitReactDirection::Right);
		}
	}
}

void ACharacterBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove)
{
	// we check if the ASC is still valid or if the current actor has authority to perform this action from a networking perspective
	if (!ASC.Get() || !HasAuthority())
	{
		return;
	}
	// we just loop through all the handles for the abilities to remove in this instance
	for (FGameplayAbilitySpecHandle& AbilityHandle : AbilitiesToRemove)
	{
		ASC->ClearAbility(AbilityHandle);
	}
}

void ACharacterBase::Die()
{
	if (ASC.IsValid())
	{
		ASC->CancelAllAbilities(); 
		ASC->AddLooseGameplayTag(DeadTag); // we add the dead tag to tell the ASC to essentially say, hey we are dead,  so don't activate any abilities that have this tag
	}
	if (DeathMontage)
	{
		if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeathMontage))
		{
			GetMesh()->PlayAnimation(DeathMontage, false);  
		}
	}
	else
	{
		FinishDying(); 
	}
}

void ACharacterBase::FinishDying()
{
	Destroy();  // we destroy the actor here. if we are doing object pooling, we wouldn't destroy them and instead set them not in use
}

float ACharacterBase::GetHealth() const
{
	if (AttributeSet.Get())
	{
		return AttributeSet->GetHealth(); 
	}
	return 0.0f;
}

bool ACharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}
