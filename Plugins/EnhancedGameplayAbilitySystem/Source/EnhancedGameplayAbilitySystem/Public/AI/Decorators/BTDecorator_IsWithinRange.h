// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsWithinRange.generated.h"

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UBTDecorator_IsWithinRange : public UBTDecorator
{
	GENERATED_BODY()
public: 
	UBTDecorator_IsWithinRange(); 
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	virtual FString GetStaticDescription() const override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector IdealRangeKey; 
	
	UPROPERTY(EditAnywhere, Category = "Keys")
	FBlackboardKeySelector TargetKey;
	
	UPROPERTY(EditAnywhere, Category = "Keys", meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float ErrorMargin; 
};


