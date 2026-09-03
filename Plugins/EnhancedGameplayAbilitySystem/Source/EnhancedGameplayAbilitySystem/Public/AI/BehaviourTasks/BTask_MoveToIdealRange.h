// 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTask_MoveToIdealRange.generated.h"

class AAIController;

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UBTask_MoveToIdealRange : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	UBTask_MoveToIdealRange();


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual FString GetStaticDescription() const override;
	
protected:
	// for filtering these in the editor so we don't grab all the keys in the blackboard, 
	// this has been setup in the constructor as we can't use the meta way of handling this 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Keys")
	FBlackboardKeySelector IdealRangeKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Keys")
	FBlackboardKeySelector TargetKey;
	
private:
	// we cache all these here, so we can terminate the node correctly
	UPROPERTY()
	TWeakObjectPtr<AAIController> CachedController;
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedComp;
	
	UPROPERTY()
	FAIRequestID CachedID;
	
	
	UFUNCTION()
	void MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
