
#include "Components/DamageWidgetComponent.h"

//plugins
#include "Kismet/KismetMathLibrary.h"
#include "UI/Components/DamageNumberWidget.h"

UDamageWidgetComponent::UDamageWidgetComponent(): TimeToLive(0.1f)
{
	SetWidgetSpace(EWidgetSpace::World); 
	SetDrawSize(FVector2D(200.f,200.f));
	SetTwoSided(false); 
	SetVisibility(true); 
	SetHiddenInGame(false);  
}

void UDamageWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
	MoveDirection = UKismetMathLibrary::RandomUnitVector();
	MoveSpeed = UKismetMathLibrary::RandomFloatInRange(MinMoveSpeed, MaxMoveSpeed);
}

void UDamageWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetWorldLocation(GetComponentLocation() + MoveDirection);
}


void UDamageWidgetComponent::SetDamageText(const float InDamage)
{
	UDamageNumberWidget* DamageNumberWidget = Cast<UDamageNumberWidget>(GetUserWidgetObject());
	if (!DamageNumberWidget) return;
	DamageNumberWidget->SetDamageNumberText(InDamage);
	GetWorld()->GetTimerManager().SetTimer(LiveTimer,[this]()
	{
		GetWorld()->GetTimerManager().ClearTimer(LookAtTimer);
		DestroyComponent();
	}, TimeToLive, false); 
}

