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
		UE_LOG(LogTemp, Warning,TEXT("%s: Behaviour Tree stopped running"), *GetName()) 
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
	if (!BehaviourTree) return; 
	RunBehaviorTree(BehaviourTree);
	
	// we want to grab the attack radius and defend radius of the enemy, we make temp values so we aren't storing these 
	// since they are one off grabs for the controller
	if (GetPawn()->GetClass()->ImplementsInterface(UEnemyAIInterface::StaticClass()))
	{
		float TempAttackRadius;
		float TempDefendRadius;
		const AActor* Actor = GetPawn(); 
		IEnemyAIInterface::Execute_GetIdealRange(Actor, TempAttackRadius, TempDefendRadius);
		Blackboard->SetValueAsFloat(AttackRadiusKeyName, TempAttackRadius); 
		Blackboard->SetValueAsFloat(DefendRadiusKeyName, TempDefendRadius);
	}
	
}

// whenever a new stimulus is updated on the perception component, this function is called 
// depending on what that stimulus is depends on how we handle that. in this case we are only checking if it was the sight, hearing or taking damage that was detected
// the idea is to loop through all the actors it has found and then when it has found a matching sense, it will update this and then move onto the next actor 
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

// this is how we go about checking what sense it was that detected the character
void AEnemyControllerBase::CanSenseActor(AActor* Actor, EAISenses SenseType, bool& OutSensed, FAIStimulus& OutStimulus)
{
	// first we grab the perception information from the perception component, as this will be what is getting used to determine what sense was detected
	FActorPerceptionBlueprintInfo PerceptionInfo;
	AIPerception->GetActorsPerception(Actor, PerceptionInfo); 
	for (const FAIStimulus& Stimulus: PerceptionInfo.LastSensedStimuli) // .we loop through all the percived perceptions, as there could be multiple senses triggered
	{
		TSubclassOf<UAISense> FoundSense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus); // we use the subclass to check what sense was triggered
		
		// we do a switch only for the sense we are looking for, and if that sense is true we set the out stimulus to the one found
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

// if the character was seen, this is how we handle what we do in each state for the AI
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

// what we want to do when the AI has "heard" something
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


