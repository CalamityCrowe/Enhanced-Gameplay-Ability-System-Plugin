// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/SendGameplayEventAnimNotify.h"

// engine
#include "AbilitySystemBlueprintLibrary.h"

// we dont need the constructor in this case, but this does remove the chance of crashing due to sending an event to an invalid component (Ability system component)
USendGameplayEventAnimNotify::USendGameplayEventAnimNotify()
{
#if WITH_EDITOR
	bShouldFireInEditor = false; 
#endif
	
}

void USendGameplayEventAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),EventTag,FGameplayEventData()); 
}

FString USendGameplayEventAnimNotify::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
		return FString::Printf(TEXT("Send Gameplay Event: %s"), *EventTag.ToString());
	
	return FString("Send Gameplay Event: N/A");
}
