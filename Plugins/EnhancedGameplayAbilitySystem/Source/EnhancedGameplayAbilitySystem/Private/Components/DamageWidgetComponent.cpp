
#include "Components/DamageWidgetComponent.h"

//plugins
#include "Kismet/KismetMathLibrary.h"
#include "UI/Components/DamageNumberWidget.h"

UDamageWidgetComponent::UDamageWidgetComponent(): TimeToLive(0.1f)
{
	SetWidgetSpace(EWidgetSpace::World); // in editor, the sample component has this set as scene  
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
	GetWorld()->GetTimerManager().SetTimer(MoveTimer,this, &UDamageWidgetComponent::MoveWidget, 1.f/60.f, true);
}

void UDamageWidgetComponent::DestroyComponent(bool bPromoteChildren)
{
	GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
	Super::DestroyComponent(bPromoteChildren);
}

void UDamageWidgetComponent::SetDamageText(const float InDamage)
{
	UDamageNumberWidget* DamageNumberWidget = Cast<UDamageNumberWidget>(GetUserWidgetObject());
	if (!DamageNumberWidget) return;
	DamageNumberWidget->SetDamageNumberText(InDamage);
	GetWorld()->GetTimerManager().SetTimer(LiveTimer,[this]()
	{
		DestroyComponent();
	}, TimeToLive, false); 
}

void UDamageWidgetComponent::MoveWidget()
{
	SetWorldLocation(GetComponentLocation() + MoveDirection);
}

