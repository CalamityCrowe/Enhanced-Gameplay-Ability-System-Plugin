// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/States/AIStates.h"
#include "BTask_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UBTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	
	UBTask_SetMovementSpeed(); 
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual FString GetStaticDescription() const override;
	
protected: 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Movement")
	EMovementSpeed MovementSpeed; 
};
