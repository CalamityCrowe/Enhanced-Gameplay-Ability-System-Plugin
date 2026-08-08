// 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "AnimNotifyState_HitScanWindow.generated.h"

/**
 * 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UAnimNotifyState_HitScanWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	
	UAnimNotifyState_HitScanWindow(); 
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected: 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities|Events", meta = (ToolTip = "The tag that is getting sent to indicate that the hit scan has started"))
	FGameplayTag HitScanStartTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities|Events", meta = (ToolTip = "The Tag that is getting sent to indicate that the hit scan has stopped"))
	FGameplayTag HitScanEndTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities|Hit Scan Properties", meta = (ToolTip = "Do we want to use the owners skeleton for the hit scan? "))
	bool bUseSkeletonForCollision = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities|Hit Scan Properties", meta = (ToolTip = "The socket on the skeleton that the hit scan will be located", EditConditions = "bUseSkeletonForCollision", EditHideConditions = true))
	FName SocketName = NAME_None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities|Hit Scan Properties", meta = (ToolTip = "The radius of the hit scan getting performed", EditConditions = "bUseSkeletonForCollision", EditHideConditions = true, UIMin = 0.0f, ClampMin = 0.0f))
	float CollisionRadius =0.0f; 	
	
	virtual FString GetNotifyName_Implementation() const override;
};
