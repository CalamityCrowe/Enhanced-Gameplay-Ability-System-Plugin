// 


#include "UI/Components/EnhancedAbilityWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/Image.h"

#include "GAS/Ability/EnhancedGameplayAbility.h"

UEnhancedAbilityWidget::UEnhancedAbilityWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	
}

void UEnhancedAbilityWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwningPlayerPawn()))
	{
		TArray<FGameplayAbilitySpecHandle> AbilityHandles;
		ASC->FindAllAbilitiesWithTags(AbilityHandles,AbilitiesToFind); 
		
		if (AbilityHandles.Num() == 0) return;
		
		bool bIsInstance;
		AbilityRef = Cast<UEnhancedGameplayAbility>(UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(ASC, AbilityHandles[0], bIsInstance));
		if (AbilityRef && AbilityRef->ShouldShowInHUD())
		{
			SetAbilityImages(*AbilityRef); 
			
			CooldownTag = AbilityRef->GetCooldownTag(); 
			ActiveTag = AbilityRef->GetActivationOwnedTags().First();
			ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::CheckForCooldown); 
		}
		
		
	}
	
}

void UEnhancedAbilityWidget::SetAbilityImages(const UEnhancedGameplayAbility& Ability)
{
	UAbilityHUDData* AbilityHUDData = Ability.GetHUDData(); 
	if (!AbilityHUDData)return; 
	
	AbilityIcon->SetBrush(AbilityHUDData->AbilityIcon); 
	AbilityIcon->SetColorAndOpacity(AbilityHUDData->DefaultColor); 
	
	ActiveIcon->SetBrush(AbilityHUDData->AbilityIcon);
	AbilityIcon->SetColorAndOpacity(AbilityHUDData->ActiveColor);
	
	CooldownIcon->SetBrush(AbilityHUDData->AbilityIcon); 
	CooldownIcon->SetColorAndOpacity(AbilityHUDData->CooldownColor);
}

void UEnhancedAbilityWidget::CheckForCooldown(FGameplayTag ChangedTag, int32 NewCount)
{
	
}
