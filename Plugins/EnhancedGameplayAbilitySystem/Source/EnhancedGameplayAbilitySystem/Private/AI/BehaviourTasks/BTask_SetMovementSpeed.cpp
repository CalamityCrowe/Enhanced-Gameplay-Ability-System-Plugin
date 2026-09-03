// 


#include "AI/BehaviourTasks/BTask_SetMovementSpeed.h"
#include "AIController.h"
#include "Interfaces/EnemyAIInterface.h"

UBTask_SetMovementSpeed::UBTask_SetMovementSpeed()
{
	NodeName = "Set Movement Speed";
}

// we check if the owning actor implements the EnemyAI interface and if so, set the speed of the character based on the enum we provide on the node
EBTNodeResult::Type UBTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* TestActor = OwnerComp.GetAIOwner()->GetPawn();
	
	if (AActor* OwningActor = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (OwningActor->GetClass()->ImplementsInterface(UEnemyAIInterface::StaticClass()))
		{
			float SpeedValue;
			IEnemyAIInterface::Execute_SetMovementSpeed(OwningActor, MovementSpeed,SpeedValue);
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("%s, Movement Speed: %.1f"),*OwningActor->GetName(), SpeedValue); // just for debugging
#endif
			return EBTNodeResult::Succeeded; 
		}
	}
	
	return EBTNodeResult::Failed;
}

FString UBTask_SetMovementSpeed::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s\n\nMovement Speed: %s"),*Super::GetStaticDescription(), *UEnum::GetDisplayValueAsText(MovementSpeed).ToString());
}
