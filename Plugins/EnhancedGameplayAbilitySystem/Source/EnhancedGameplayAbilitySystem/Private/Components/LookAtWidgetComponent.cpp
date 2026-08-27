// 


#include "Components/LookAtWidgetComponent.h"

#include "Kismet/KismetMathLibrary.h"


ULookAtWidgetComponent::ULookAtWidgetComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	
}

void ULookAtWidgetComponent::DestroyComponent(bool bPromoteChildren)
{
	GetWorld()->GetTimerManager().ClearTimer(LookAtTimerHandle);  // we clear the handle here to prevent it attempting to run on a deleted component
	Super::DestroyComponent(bPromoteChildren);
}

// need to look at this to see if I can mark it as const and get rid of member variable
void ULookAtWidgetComponent::SetLookAtTarget(USceneComponent* InTarget)
{
	LookAtTarget = InTarget;
	GetWorld()->GetTimerManager().ClearTimer(LookAtTimerHandle);// clear just in case it is already running 
	GetWorld()->GetTimerManager().SetTimer(LookAtTimerHandle, [this]()
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), LookAtTarget->GetComponentLocation());
		LookAtRot.Roll = 0.0f;
		SetWorldRotation(LookAtRot);
	}, LookAtInterval, true); 
}


