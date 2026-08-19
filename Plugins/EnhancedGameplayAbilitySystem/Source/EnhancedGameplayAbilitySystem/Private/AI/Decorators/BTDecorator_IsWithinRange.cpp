// 


#include "AI/Decorators/BTDecorator_IsWithinRange.h"

//engine
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsWithinRange::UBTDecorator_IsWithinRange()
{
	NodeName = "Is Within Range";
	
	TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsWithinRange,TargetKey), AActor::StaticClass());
	IdealRangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_IsWithinRange, IdealRangeKey)); 
}

bool UBTDecorator_IsWithinRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	
	if (!BlackboardComponent ||!OwningPawn)return false;
	if (TargetKey.SelectedKeyName.IsNone() ||IdealRangeKey.SelectedKeyName.IsNone()) return false;
	
	
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor) return false;
	
	float Distance = (OwningPawn->GetDistanceTo(TargetActor) - ErrorMargin);
	float IdealRange = BlackboardComponent->GetValueAsFloat(IdealRangeKey.SelectedKeyName); 
	
	return (Distance <= IdealRange);
}

FString UBTDecorator_IsWithinRange::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s\nTarget Key: %s\nIdeal Range: %s\nError Margin: %.1f"),
	 *Super::GetStaticDescription(),
	 *TargetKey.SelectedKeyName.ToString(),
	 *IdealRangeKey.SelectedKeyName.ToString(),
	 ErrorMargin);
	
}

