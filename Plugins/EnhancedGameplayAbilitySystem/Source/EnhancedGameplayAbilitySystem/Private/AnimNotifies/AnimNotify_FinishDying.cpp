// 


#include "AnimNotifies/AnimNotify_FinishDying.h"

#include "Characters/CharacterBase.h"

UAnimNotify_FinishDying::UAnimNotify_FinishDying()
{
#if WITH_EDITOR
	bShouldFireInEditor = false;
	NotifyColor = FColor(255,0,0);
#endif
}

void UAnimNotify_FinishDying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (ACharacterBase* OwningCharacter = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		OwningCharacter->FinishDying(); 
	}
#if WITH_EDITOR
	else
	{
		UE_LOG(LogTemp, Warning,TEXT("Attempted to use %s on an actor that doesn't inherit from character base: %s"), *GetNotifyName(), *MeshComp->GetOwner()->GetName()); 
	}
#endif
}

FString UAnimNotify_FinishDying::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Finish Dying"));
}
