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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Keys")
	FBlackboardKeySelector IdealRangeKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Keys")
	FBlackboardKeySelector TargetKey;
	
private:
	UPROPERTY()
	TWeakObjectPtr<AAIController> CachedController;
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedComp;
	
	UPROPERTY()
	FAIRequestID CachedID;
	
	
	UFUNCTION()
	void MoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
};
