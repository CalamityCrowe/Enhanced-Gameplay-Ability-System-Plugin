
#include "GAS/Ability/Movement/MovementAbility.h"

//engine
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/CharacterMovementComponent.h"

// plugin
#include "Characters/CharacterBase.h"


UMovementAbility::UMovementAbility():Strength(0.0f), Duration(0.0f), bIsAdditive(false), 
                                     StrengthOverTimeCurve(nullptr),VelocityOnFinishMode(ERootMotionFinishVelocityMode::ClampVelocity),bEnableGravity(false)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; 
}

void UMovementAbility::MontageStarted_Implementation()
{
	Super::MontageStarted_Implementation();
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		EventTriggerTag,
		nullptr,
		true,
		true); 
	
	EventTask->EventReceived.AddDynamic(this, &UMovementAbility::OnEventTriggered); 
	EventTask->ReadyForActivation(); 
}

void UMovementAbility::OnEventTriggered(FGameplayEventData Payload)
{
	Super::OnEventTriggered(Payload);
	
	// since this is a generic dash ability for here, we won't apply anything here at the moment.
	// However, we could extend this if we wanted to, by creating a function similar to montage started
	
	FVector WorldDirection = GetMoveDirection();
	float ClampSpeed = GetMaxSpeed();
	
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Green,WorldDirection.ToString());
	
	
	UAbilityTask_ApplyRootMotionConstantForce* Moveforce = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this,
		FName(""),WorldDirection,
		Strength,Duration,
		bIsAdditive, StrengthOverTimeCurve,
		VelocityOnFinishMode, FVector(0,0,0), 
		ClampSpeed, bEnableGravity
		);
	
	Moveforce->OnFinish.AddDynamic(this, &UMovementAbility::OnDashFinish);
	Moveforce->ReadyForActivation();
}

void UMovementAbility::OnDashFinish()
{
	//CommitAbilityCooldown(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true); 
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true, false); 
}

float UMovementAbility::GetMaxSpeed() const
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo()))
	{
		return Character->GetCharacterMovement()->MaxWalkSpeed; 
	}
	return 0.0f; // we will return a value of 0 if for whatever reason the casting fails to find a character
}

FVector UMovementAbility::GetMoveDirection() const
{
	AActor* OwningActor = GetAvatarActorFromActorInfo();
	if (ACharacterBase* Character = Cast<ACharacterBase>(OwningActor))
	{
		if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		{
			FVector Velocity = MovementComponent->Velocity.GetSafeNormal();
			Velocity.Z = 0.0f;
			if (Velocity != FVector::ZeroVector)
			{
				return Velocity;
			}			
		}
		FVector ControlDirection = Character->GetControlRotation().Vector();
		ControlDirection.Z = 0;
		
		return bMovetoCameraDirection?ControlDirection: Character->GetActorForwardVector().GetSafeNormal(); 
	}
	return OwningActor->GetActorForwardVector().GetSafeNormal();
};
