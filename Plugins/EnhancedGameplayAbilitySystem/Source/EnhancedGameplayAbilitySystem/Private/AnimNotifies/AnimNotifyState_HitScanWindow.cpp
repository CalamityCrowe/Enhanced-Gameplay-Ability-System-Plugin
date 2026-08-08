// 


#include "AnimNotifies/AnimNotifyState_HitScanWindow.h"
//engine
#include "AbilitySystemBlueprintLibrary.h"
//plugin
#include "GAS/Data/SocketCollisionParamsObject.h"

UAnimNotifyState_HitScanWindow::UAnimNotifyState_HitScanWindow()
{
#if WITH_EDITOR
	bShouldFireInEditor = false;
#endif
}

void UAnimNotifyState_HitScanWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	FGameplayEventData Payload;
	if (bUseSkeletonForCollision)
	{
		USocketCollisionParamsObject* ParamsObject = NewObject<USocketCollisionParamsObject>();
		ParamsObject->Params.SocketName = SocketName;
		ParamsObject->Params.CollisionRadius = CollisionRadius;
		
		Payload.OptionalObject = ParamsObject;
	}
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), HitScanStartTag, Payload);
}

void UAnimNotifyState_HitScanWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),HitScanEndTag, FGameplayEventData()); 
}

FString UAnimNotifyState_HitScanWindow::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Hit Scan Window"));
}
