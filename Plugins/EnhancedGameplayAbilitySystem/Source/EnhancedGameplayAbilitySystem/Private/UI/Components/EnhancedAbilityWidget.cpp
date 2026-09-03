// 


#include "UI/Components/EnhancedAbilityWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

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
			ASC->RegisterGameplayTagEvent(ActiveTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::UpdateActiveWidget); 
		}
	}
	
}

// setting up the display of the widget for the ability here, essentially just grabs the icon for the ability and what colour each state should be 
void UEnhancedAbilityWidget::SetAbilityImages(const UEnhancedGameplayAbility& Ability) const
{
	UAbilityHUDData* AbilityHUDData = Ability.GetHUDData(); 
	if (!AbilityHUDData)return; 
	
	AbilityIcon->SetBrush(AbilityHUDData->AbilityIcon); 
	AbilityIcon->SetColorAndOpacity(AbilityHUDData->DefaultColor); 
	
	ActiveIcon->SetBrush(AbilityHUDData->AbilityIcon);
	ActiveIcon->SetColorAndOpacity(AbilityHUDData->ActiveColor);
	
	CooldownIcon->SetBrush(AbilityHUDData->AbilityIcon); 
	CooldownIcon->SetColorAndOpacity(AbilityHUDData->CooldownColor);
	
	ActiveOverlay->SetVisibility(ESlateVisibility::Collapsed);
	CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

// when the ability goes on cooldown, a timer is set that decreases that is checked at a fixed time of a tenth of a second
void UEnhancedAbilityWidget::CheckForCooldown(FGameplayTag ChangedTag, int32 NewCount)
{
	if (!AbilityRef) return; 
	const float Remaining = AbilityRef->GetCooldownTimeRemaining(); 
	const bool bIsTimerActive  = GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle); 
	
	if (Remaining> 0 && !bIsTimerActive)
	{
		CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,this,&ThisClass::UpdateCooldownProgress, 0.1f, true); 
	}
}

// checks if the ability is active based on the tag. if so it will render the active widget
void UEnhancedAbilityWidget::UpdateActiveWidget(FGameplayTag ChangedTag,int32 NewCount ) const
{
	const ESlateVisibility NewVisibility = NewCount > 0? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	ActiveOverlay->SetVisibility(NewVisibility);
}

// we update how long is remaining before we can use the ability again, once it is off cooldown, the widget will indicate it is available again 
void UEnhancedAbilityWidget::UpdateCooldownProgress()
{
	if (!AbilityRef) return; 
	float RemainingCooldown = AbilityRef->GetCooldownTimeRemaining();
	
	if (RemainingCooldown > 0.0f)
	{
		const FText TempCooldownText = UTextFormattingHelper::FormatFloatWithCustomOptions(RemainingCooldown, CooldownTextFormat);
		CooldownText->SetText(TempCooldownText); 
	}
	else
	{
		CooldownText->SetText(FText::GetEmpty());
		CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}
