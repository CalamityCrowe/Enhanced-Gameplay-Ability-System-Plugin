// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "SendGameplayEventAnimNotify.generated.h"


/**
 *  a generic class to handle the sending of gameplay events to the owning actor when they are meant to trigger in the animation
 *  
 *  you would use this to signal to the character to say fire a gun or spawn an aoe damage. 
 */
UCLASS()
class ENHANCEDGAMEPLAYABILITYSYSTEM_API USendGameplayEventAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	USendGameplayEventAnimNotify();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	FGameplayTag EventTag;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	virtual FString GetNotifyName_Implementation() const override;
};
