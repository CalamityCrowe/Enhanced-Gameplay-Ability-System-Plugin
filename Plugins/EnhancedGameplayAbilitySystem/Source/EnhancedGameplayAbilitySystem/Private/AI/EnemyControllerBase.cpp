// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyControllerBase.h"

//engine
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

// plugin
#include "Interfaces/EnemyAIInterface.h"


AEnemyControllerBase::AEnemyControllerBase()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &ThisClass::UpdatePerception); 
}

void AEnemyControllerBase::StopBehaviourTree()
{
	if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple,"Behavior tree stopped running"); 
#endif
		BTComp->StopTree(); 
	}
}

EAIStates AEnemyControllerBase::GetCurrentState() const
{
	return static_cast<EAIStates>(Blackboard->GetValueAsEnum(AIStateKeyName));
}

void AEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviourTree)
	{
		RunBehaviorTree(BehaviourTree);
		
		if (GetPawn()->GetClass()->ImplementsInterface(UEnemyAIInterface::StaticClass()))
		{
			float TempAttackRadius;
			float TempDefendRadius;
			AActor* Actor = GetPawn(); 
			IEnemyAIInterface::Execute_GetIdealRange(Actor, TempAttackRadius, TempDefendRadius);
			Blackboard->SetValueAsFloat(AttackRadiusKeyName, TempAttackRadius); 
			Blackboard->SetValueAsFloat(DefendRadiusKeyName, TempDefendRadius);
		}
	}
}

void AEnemyControllerBase::UpdatePerception(const TArray<AActor*>& Actors)
{
	bool bSensed = false; 
	FAIStimulus TempStimulus;
	for (AActor* Actor : Actors)
	{
		bSensed = false;
		CanSenseActor(Actor, EAISenses::Sight, bSensed, TempStimulus);
		if (bSensed)
		{
			HandleSensedSight(Actor, TempStimulus);
			continue; 
		}
		CanSenseActor(Actor, EAISenses::Hearing, bSensed, TempStimulus);
		if (bSensed)
		{
			HandleSensedSound(TempStimulus.StimulusLocation);  
			continue;
		}
		CanSenseActor(Actor, EAISenses::Damage, bSensed, TempStimulus);
		if (bSensed)
		{
			// no idea what I am doing for damage here
		}
	}
	
	
}

void AEnemyControllerBase::CanSenseActor(AActor* Actor, EAISenses SenseType, bool& OutSensed, FAIStimulus& OutStimulus)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	AIPerception->GetActorsPerception(Actor, PerceptionInfo); 
	for (const FAIStimulus& Stimulus: PerceptionInfo.LastSensedStimuli)
	{
		TSubclassOf<UAISense> FoundSense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
		switch (SenseType)
		{
		case EAISenses::Sight:
			if (FoundSense == UAISense_Sight::StaticClass())
			{
				OutSensed = Stimulus.WasSuccessfullySensed(); 
				OutStimulus = Stimulus;
			}
			break;
		case EAISenses::Hearing:
			if (FoundSense == UAISense_Hearing::StaticClass())
			{
				OutSensed = Stimulus.WasSuccessfullySensed();
				OutStimulus = Stimulus;
			}
			break;
		case EAISenses::Damage:
			if (FoundSense == UAISense_Damage::StaticClass())
			{
				OutSensed = Stimulus.WasSuccessfullySensed();
				OutStimulus = Stimulus;
			}
			break; 
		default:
			break;
		}
	}
}

void AEnemyControllerBase::HandleSensedSight(AActor* Actor, const FAIStimulus& Stimulus)
{
	switch (GetCurrentState())
	{
		// need to look at team AI assignment
	case EAIStates::Passive:
		SetStateAsAttacking(Actor); 
		break;
	case EAIStates::Investigating:
		SetStateAsAttacking(Actor);
		break; 
	default:
		break; 
	}
}

void AEnemyControllerBase::HandleSensedSound(const FVector& SoundLocation)
{
	switch (GetCurrentState())
	{
	case EAIStates::Passive:
		SetStateAsInvestigating(SoundLocation); 
		break;
	default:
		break;
	}
}


void AEnemyControllerBase::SetStateAsAttacking(AActor* Target) // can't be made const due to assigning the TargetActor in the controller
{
	if (!Target) return; 
	TargetActor = Target;
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8>(EAIStates::Attacking));
	Blackboard->SetValueAsObject(TargetKeyName, Target);
}

void AEnemyControllerBase::SetStateAsInvestigating(const FVector& Location) const // made const as it doesn't modify a value in the controller
{
	Blackboard->SetValueAsVector(PointOfInterestKeyName, Location);
	Blackboard->SetValueAsEnum(AIStateKeyName, static_cast<uint8> (EAIStates::Investigating));
}


