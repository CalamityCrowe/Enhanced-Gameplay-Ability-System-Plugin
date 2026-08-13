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

void UEnhancedAbilityWidget::SetAbilityImages(const UEnhancedGameplayAbility& Ability)
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

void UEnhancedAbilityWidget::CheckForCooldown(FGameplayTag ChangedTag, int32 NewCount)
{
	if (!AbilityRef) return; 
	float Remaining = AbilityRef->GetCooldownTimeRemaining(); 
	bool bIsTimerActive  = GetWorld()->GetTimerManager().IsTimerActive(CooldownTimerHandle); 
	
	if (Remaining> 0 && !bIsTimerActive)
	{
		CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,this,&ThisClass::UpdateCooldownProgress, 0.1f, true); 
	}
}

void UEnhancedAbilityWidget::UpdateActiveWidget(FGameplayTag ChangedTag,int32 NewCount )
{
	const ESlateVisibility NewVisibility = NewCount > 0? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	ActiveOverlay->SetVisibility(NewVisibility);
}

void UEnhancedAbilityWidget::UpdateCooldownProgress()
{
	if (!AbilityRef) return; 
	float RemainingCooldown = AbilityRef->GetCooldownTimeRemaining();
	
	if (RemainingCooldown > 0.0f)
	{
		FNumberFormattingOptions CooldownTextFormat;
		CooldownTextFormat.SetUseGrouping(true);
		CooldownTextFormat.RoundingMode = HalfToEven; 
		CooldownTextFormat.MinimumIntegralDigits= 1;
		CooldownTextFormat.MaximumIntegralDigits= 2;
		CooldownTextFormat.MinimumFractionalDigits = 0;
		CooldownTextFormat.MaximumFractionalDigits = 1;

		const FText AsText = FText::AsNumber(RemainingCooldown, &CooldownTextFormat);
		CooldownText->SetText(AsText); 
	}
	else
	{
		CooldownText->SetText(FText::GetEmpty());
		CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}
