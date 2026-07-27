// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EnhancedAbilitySystemComponent.h"

#include "GAS/Ability/EnhancedGameplayAbility.h"

UEnhancedAbilitySystemComponent::UEnhancedAbilitySystemComponent()
{
	ClearAbilityInput(); 
}

void UEnhancedAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	// we check if the input tag that has been passed in is valid in this context
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec: ActivatableAbilities.Items) // we then loop through all the abilities in the component 
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))) // we then check if the ability is valid and if it has the same input tag as the one passed in
			{
				// if so, we add it to the list of held and pressed abilities
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UEnhancedAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec: ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTag(InputTag)))
			{
				// this is the same idea as the pressed logic, but instead we add the input tag to the released array and remove it from the held
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UEnhancedAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate; 
	AbilitiesToActivate.Reset(); 
	
	// we first process all the held inputs
	for (const FGameplayAbilitySpecHandle& SpecHandle: InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle)) // we attempt to find the ability from the spec handle in the held inputs
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive()) // next we check if it is valid and it is not active at the moment
			{
				const UEnhancedGameplayAbility* EnhancedAbility = CastChecked<UEnhancedGameplayAbility>(AbilitySpec->Ability); // we do a cast check here as if for whatever reason this is not a valid check, we want it to crash/ log it 
				if (EnhancedAbility->GetActivationPolicy() == EEnhancedActivationPolicy::WhileInputActive) // we check if the activation policy is held and if so add it to the list of abilities to activate
				{
					AbilitiesToActivate.Add(SpecHandle);
				}
			}
		}
	}
	// next we process the pressed inputs that weren't the held variant
	for (const FGameplayAbilitySpecHandle& SpecHandle: InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle)) // we grab the ability spec from the spec handle, we don't make it const this time as we need to change an input flag
		{
			if (AbilitySpec->Ability) // we check if the ability is valid
			{
				AbilitySpec->InputPressed = true;  // we tell it this ability is pressed at the moment
				
				const UEnhancedGameplayAbility* EnhancedAbility = CastChecked<UEnhancedGameplayAbility>(AbilitySpec->Ability); // we grab the ability here so we can check the activation policy 
				
				if (AbilitySpec->IsActive())
				{
					UE_LOG(LogTemp, Warning, TEXT("Ability is active on press - Policy: %d"), (int32)EnhancedAbility->GetActivationPolicy());
					if (EnhancedAbility->GetActivationPolicy() == EEnhancedActivationPolicy::Toggle)
					{
						UE_LOG(LogTemp, Warning, TEXT("Toggle cancel triggered for: %s"), *AbilitySpec->Ability->GetName());
						CancelAbilityHandle(SpecHandle);
					}
					else
					{
						AbilitySpecInputPressed(*AbilitySpec);
					}
				}
				else // otherwise we cast check it and add it to the abilities to activate
				{
					if (EnhancedAbility->GetActivationPolicy() == EEnhancedActivationPolicy::OnInputTriggered||
						EnhancedAbility->GetActivationPolicy() == EEnhancedActivationPolicy::Toggle)
					{
						AbilitiesToActivate.Add(SpecHandle);
					}
				}
			}
		}
	}
	
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle: AbilitiesToActivate) // we loop through all the abilities waiting to activate and then attempt to activate them
	{
		TryActivateAbility(AbilitySpecHandle);
	}
	
	// lastly we go through all the inputs that have been released
	for ( const FGameplayAbilitySpecHandle& AbilitySpecHandle: InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle)) // we grab the ability specs from the handles
		{
			if (AbilitySpec->Ability) // we check if the ability is valid
			{
				AbilitySpec->InputPressed = false;  // we then release the input of the spec
				
				if (AbilitySpec->IsActive()) // if is active, we then call the overriden function to release inputs from ability specs
				{
					AbilitySpecInputReleased(*AbilitySpec); 
				}
			}
		}
	}
	
	// we reset the pressed and released buttons as these should be empty at the start of the function again
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UEnhancedAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

bool UEnhancedAbilitySystemComponent::IsAbilityActive(const FGameplayTagContainer& WithTags,
	const FGameplayTagContainer& WithoutTags, UEnhancedGameplayAbility* Ignore)
{
	ABILITYLIST_SCOPE_LOCK(); 
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (!Spec.IsActive() || Spec.Ability == nullptr || Spec.Ability == Ignore)
		{
			continue;
		}
		
		const bool WithTagPass = (Spec.Ability->GetAssetTags().HasAny(WithTags)); 
		const bool WithoutTagPass = (!Spec.Ability->GetAssetTags().HasAny(WithoutTags));
		if (WithTagPass && WithoutTagPass)
		{
			return true;
		}
	}
	return false;
}

bool UEnhancedAbilitySystemComponent::IsAbilityActive(TSubclassOf<UEnhancedGameplayAbility> AbilityClass)
{
	ABILITYLIST_SCOPE_LOCK(); 
	FGameplayAbilitySpec* Spec;
	if (!AbilityClass)
	{
		return false;
	}
	Spec = FindAbilitySpecFromClass(AbilityClass); 
	if (Spec)
	{
		return Spec->IsActive();
	}
	return false; 
}

void UEnhancedAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);
	if (Spec.IsActive())
	{
		const UGameplayAbility* Ability = Spec.GetPrimaryInstance(); // gets the ability that has been assigned by the spec for instanced actor
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
				// we check if the ability is null or not and base the activation event from the spec if it is, otherwise we pull it from the ability (value == condition? true:false)
				const FPredictionKey PredictionKey = Ability == nullptr? Spec.ActivationInfo.GetActivationPredictionKey():Ability->GetCurrentActivationInfo().GetActivationPredictionKey(); 
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, PredictionKey); // we then Invoke the event for the pressed input here
	}
}

void UEnhancedAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	if (Spec.IsActive())
	{
		const UGameplayAbility* Ability = Spec.GetPrimaryInstance();
		PRAGMA_DISABLE_DEPRECATION_WARNINGS		
				const FPredictionKey PredictionKey = Ability == nullptr? Spec.ActivationInfo.GetActivationPredictionKey():Ability->GetCurrentActivationInfo().GetActivationPredictionKey(); 
		PRAGMA_ENABLE_DEPRECATION_WARNINGS		
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, PredictionKey);
	}
}