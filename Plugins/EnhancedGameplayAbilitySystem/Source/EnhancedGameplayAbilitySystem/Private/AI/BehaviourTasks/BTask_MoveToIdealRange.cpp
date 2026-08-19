// 


#include "AI/BehaviourTasks/BTask_MoveToIdealRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTask_MoveToIdealRange::UBTask_MoveToIdealRange()
{
	NodeName = "MoveToIdealRange";
	bCreateNodeInstance = true;
	
	IdealRangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTask_MoveToIdealRange, IdealRangeKey));
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTask_MoveToIdealRange, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTask_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent(); 
	CachedComp = &OwnerComp; 
	CachedController = OwnerComp.GetAIOwner();
	
	if (!CachedController.Get() || !BlackboardComponent) return EBTNodeResult::Failed;
	if (IdealRangeKey.SelectedKeyName.IsNone() || TargetKey.SelectedKeyName.IsNone()) return EBTNodeResult::Failed;
	
	float AcceptanceRadius = BlackboardComponent->GetValueAsFloat(IdealRangeKey.SelectedKeyName); 
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)return EBTNodeResult::Failed;
	
	FAIMoveRequest Request;
	Request.SetGoalActor(TargetActor);
	Request.SetAcceptanceRadius(AcceptanceRadius);
	Request.SetCanStrafe(false); 
	Request.SetUsePathfinding(true);
	
	FPathFollowingRequestResult RequestResult = CachedController->MoveTo(Request);
	switch (RequestResult.Code)
	{
	case EPathFollowingRequestResult::RequestSuccessful:
		
		CachedID = RequestResult.MoveId; 
		
		CachedController->ReceiveMoveCompleted.AddDynamic(this, &UBTask_MoveToIdealRange::MoveCompleted); 
		return EBTNodeResult::InProgress; 
		
	case EPathFollowingRequestResult::AlreadyAtGoal:
		return EBTNodeResult::Succeeded; 
	default:
		return EBTNodeResult::Failed;
		;
	}
}

EBTNodeResult::Type UBTask_MoveToIdealRange::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		AIController->StopMovement(); 
		AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTask_MoveToIdealRange::MoveCompleted);
	}
	return EBTNodeResult::Aborted;
}

FString UBTask_MoveToIdealRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("Moving towards the ideal range from the target\n\n%s\n\nTarget Key: %s\n Ideal Range Key: %s"),
		*Super::GetStaticDescription(),
		*TargetKey.SelectedKeyName.ToString(),
		*IdealRangeKey.SelectedKeyName.ToString());
}

void UBTask_MoveToIdealRange::MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (RequestID != CachedID)return;
	if (CachedController.Get()) CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTask_MoveToIdealRange::MoveCompleted);
	if (!CachedComp) return;
	
	EBTNodeResult::Type NodeResult = Result== EPathFollowingResult::Success? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	
	FinishLatentTask(*CachedComp, NodeResult);
}
