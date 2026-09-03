// 


#include "AI/BehaviourTasks/BTask_MoveToIdealRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTask_MoveToIdealRange::UBTask_MoveToIdealRange()
{
	NodeName = "MoveToIdealRange";
	bCreateNodeInstance = true;
	
	// since we are only wanting float or actor values for the dependant key, this is how we add a filter to these
	IdealRangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTask_MoveToIdealRange, IdealRangeKey)); 
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTask_MoveToIdealRange, TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTask_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// we grab the relevant components for the blackboard, along with caching the AI controller/behaviour tree
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();  
	CachedComp = &OwnerComp; 
	CachedController = OwnerComp.GetAIOwner();
	
	// if any of these conditions return as false, we exit out of this node as a fail
	if (!CachedController.Get() || !BlackboardComponent) return EBTNodeResult::Failed; 
	if (IdealRangeKey.SelectedKeyName.IsNone() || TargetKey.SelectedKeyName.IsNone()) return EBTNodeResult::Failed;
	
	// we grab values we will be working with for the node from the blackboard
	const float AcceptanceRadius = BlackboardComponent->GetValueAsFloat(IdealRangeKey.SelectedKeyName); 
	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)return EBTNodeResult::Failed;
	
	// setting up the AI parameters for handling the moving to the target
	FAIMoveRequest Request;
	Request.SetGoalActor(TargetActor);
	Request.SetAcceptanceRadius(AcceptanceRadius);
	Request.SetCanStrafe(false); 
	Request.SetUsePathfinding(true);
	
	// we send the request to the controller so they can move
	FPathFollowingRequestResult RequestResult = CachedController->MoveTo(Request);
	
	// we lastly check, what we want to do when the request has been processed by the controller
	switch (RequestResult.Code)
	{
	case EPathFollowingRequestResult::RequestSuccessful:
		CachedID = RequestResult.MoveId; // this will be the ID that we will be using to check for when the move has been completed
		CachedController->ReceiveMoveCompleted.AddDynamic(this, &UBTask_MoveToIdealRange::MoveCompleted); 
		return EBTNodeResult::InProgress; 
		
	case EPathFollowingRequestResult::AlreadyAtGoal:
		return EBTNodeResult::Succeeded; 
	default:
		return EBTNodeResult::Failed;
	}
}

// standard aborting for the task, we will stop the movement and clear any delegates so we don't have the same finished move function bound multiple times
EBTNodeResult::Type UBTask_MoveToIdealRange::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedController.Get())
	{
		CachedController->StopMovement(); 
		CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTask_MoveToIdealRange::MoveCompleted);
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

// what we want to happen when the movement is completed, in this instance we check if the cached ID matches the request ID
// if so, it will remove the complete request and tells the node whether it failed or not getting to the target
void UBTask_MoveToIdealRange::MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (RequestID != CachedID)return;
	if (CachedController.Get()) CachedController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTask_MoveToIdealRange::MoveCompleted);
	if (!CachedComp) return;
	
	EBTNodeResult::Type NodeResult = Result== EPathFollowingResult::Success? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	
	FinishLatentTask(*CachedComp, NodeResult);
}
