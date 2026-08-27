

#include "Components/FloatingHealthComponent.h"

#include "Kismet/KismetMathLibrary.h"


UFloatingHealthComponent::UFloatingHealthComponent()
{
	SetWidgetSpace(EWidgetSpace::World); 
	SetDrawSize(FVector2D(100.f, 30.f));
	SetTwoSided(false);
	SetVisibility(true);
}

void UFloatingHealthComponent::SetLookAtTarget(USceneComponent* InTarget)
{
	Target = InTarget;
	GetWorld()->GetTimerManager().SetTimer(LookAtTimer, [this]()
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), Target->GetComponentLocation()); 
		LookAtRotation.Roll = 0.0f; 
		SetWorldRotation(LookAtRotation);
	},0.1f, true); 
}


void UFloatingHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}


