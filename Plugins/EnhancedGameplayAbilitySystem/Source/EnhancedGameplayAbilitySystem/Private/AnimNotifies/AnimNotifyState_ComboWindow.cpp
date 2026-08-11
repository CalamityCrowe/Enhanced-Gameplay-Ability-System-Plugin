// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotifyState_ComboWindow.h"

// engine
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

UAnimNotifyState_ComboWindow::UAnimNotifyState_ComboWindow()
{
#if WITH_EDITOR
	bShouldFireInEditor = false;
#endif
	
	ComboStartTag =FGameplayTag::RequestGameplayTag(FName("Event.Combo"), false); 
	ComboEndTag = FGameplayTag::RequestGameplayTag(FName("Event.Combo"), false);
	
}

void UAnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),ComboStartTag,FGameplayEventData()); 
	
	
}

void UAnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),ComboEndTag,FGameplayEventData()); 
	
}

FString UAnimNotifyState_ComboWindow::GetNotifyName_Implementation() const
{
	if (ComboStartTag.IsValid() && ComboEndTag.IsValid())
		return FString::Printf(TEXT("Combo Start: %s , Combo End: %s"), *ComboStartTag.ToString(), *ComboEndTag.ToString());
	return FString("Combo Window: Tags Not Set");
}
