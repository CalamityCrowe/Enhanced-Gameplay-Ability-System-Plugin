// 


#include "AI/BehaviourTasks/BTask_SetMovementSpeed.h"
#include "AIController.h"
#include "Interfaces/EnemyAIInterface.h"

UBTask_SetMovementSpeed::UBTask_SetMovementSpeed()
{
	NodeName = "Set Movement Speed";
}

EBTNodeResult::Type UBTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AActor* OwningActor = OwnerComp.GetAIOwner()->GetPawn())
	{
		if (OwningActor->GetClass()->ImplementsInterface(UEnemyAIInterface::StaticClass()))
		{
			float SpeedValue;
			IEnemyAIInterface::Execute_SetMovementSpeed(OwningActor, MovementSpeed,SpeedValue);
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("Movement Speed: %.1f"), SpeedValue); // just for debugging
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
