// 


#include "Components/FloatingHealthComponent.h"

#include "AbilitySystemComponent.h"
#include "Characters/CharacterBase.h"
#include "UI/Enemy/EnhancedEnemyStatsWidget.h"

// Sets default values for this component's properties
UFloatingHealthComponent::UFloatingHealthComponent()
{
	
}


void UFloatingHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UEnhancedEnemyStatsWidget* ESWidget = Cast<UEnhancedEnemyStatsWidget>(GetWidget()); 
	if (!ESWidget) return;
	
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
	{
		OwningCharacterRef = Character;
		if (UAbilitySystemComponent* ASC = OwningCharacterRef->GetAbilitySystemComponent())
		{
			ESWidget->InitializeStats(ASC); 	// this is how we get the stat bar to update to owning enemy, instead of pulling from the player
		}
	}

}


