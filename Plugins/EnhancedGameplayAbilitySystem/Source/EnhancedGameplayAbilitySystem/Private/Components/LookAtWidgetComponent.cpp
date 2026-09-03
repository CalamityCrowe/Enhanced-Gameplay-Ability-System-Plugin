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

// this will set a timer that will set the Widget to llok towards the target that has been set
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


