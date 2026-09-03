// 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FinishDying.generated.h"

/**
 *  the notify that will signal that the actor has finished dying
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UAnimNotify_FinishDying : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_FinishDying();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected: 
	virtual FString GetNotifyName_Implementation() const override;
};
