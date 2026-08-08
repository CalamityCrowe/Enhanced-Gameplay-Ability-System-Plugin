// 


#include "GAS/Ability/Melee/UnarmedComboAbility.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

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
	
	UAbilityTask_WaitGameplayEvent* InputReceivedEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ComboInputTag, nullptr, false, true); 
	
	InputReceivedEvent->EventReceived.AddDynamic(this, &ThisClass::InputReceivedEvent);
	InputReceivedEvent->ReadyForActivation();
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

void UUnarmedComboAbility::InputReceivedEvent(FGameplayEventData Payload)
{
	bReceivedInputAtRightTime = bIsWithinComboWindow; 
}
