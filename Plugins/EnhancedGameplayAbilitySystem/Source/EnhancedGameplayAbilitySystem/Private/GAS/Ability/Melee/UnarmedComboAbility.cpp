// 


#include "GAS/Ability/Melee/UnarmedComboAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

UUnarmedComboAbility::UUnarmedComboAbility()
{
	constexpr bool bErrorIfFound = false;
	ComboStartTag = FGameplayTag::RequestGameplayTag(FName("Event.Combo.Start"), bErrorIfFound);
	ComboEndTag = FGameplayTag::RequestGameplayTag(FName("Event.Combo.End"), bErrorIfFound);
	ComboInputTag = FGameplayTag::RequestGameplayTag(FName("Event.Combo.Input"), bErrorIfFound);
}

void UUnarmedComboAbility::MontageStarted()
{
	Super::MontageStarted();
	
	ComboCount = 1; // resets the combo count here
	
	UAbilityTask_WaitGameplayEvent* ComboStartEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ComboStartTag, nullptr, false, true); 
	
	ComboStartEvent->EventReceived.AddDynamic(this, &ThisClass::ContinueComboStartEvent); 
	ComboStartEvent->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ComboEndEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ComboEndTag, nullptr,false, true);
	
	ComboEndEvent->EventReceived.AddDynamic(this, &ThisClass::ContinueComboEndEvent);
	ComboEndEvent->ReadyForActivation();
	
	WaitComboInput(); 
}

void UUnarmedComboAbility::WaitComboInput()
{
	// this wait task can only be triggered once, so we made it a function to call when we need to wait again
	UAbilityTask_WaitInputPress* InputPressed = UAbilityTask_WaitInputPress::WaitInputPress(this, false); 
	InputPressed->OnPress.AddDynamic(this, &ThisClass::InputReceivedEvent);
	InputPressed->ReadyForActivation();
}

void UUnarmedComboAbility::ContinueComboStartEvent(FGameplayEventData Payload)
{
	bIsWithinComboWindow = true;
	bReceivedInputAtRightTime = false;
}

void UUnarmedComboAbility::ContinueComboEndEvent(FGameplayEventData Payload)
{
	bIsWithinComboWindow = false;
	if (bReceivedInputAtRightTime)
	{
		ComboCount++;
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,true, false); 
	}
}

void UUnarmedComboAbility::InputReceivedEvent(float TimeWaited)
{
	bReceivedInputAtRightTime = bIsWithinComboWindow; 
	WaitComboInput(); // like here
}
