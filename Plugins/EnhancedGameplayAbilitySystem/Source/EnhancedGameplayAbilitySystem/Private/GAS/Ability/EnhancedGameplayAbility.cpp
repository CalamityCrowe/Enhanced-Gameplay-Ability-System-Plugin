// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/EnhancedGameplayAbility.h"

#include "AbilitySystemLog.h"
#include "AbilitySystemComponent.h"
#include "AssetDefinitionAssetInfo.h"
#include "GameFramework/Character.h"

UEnhancedGameplayAbility::UEnhancedGameplayAbility():ActivationPolicy(EEnhancedActivationPolicy::OnInputTriggered),bActivateOnGranted(false)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEnhancedGameplayAbility::PostInitProperties()
{
	Super::PostInitProperties();
	FGameplayTagContainer AbilityContainer;
	AbilityContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GameplayAbility"), false)); 
	SetAssetTags(AbilityContainer); 
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead",false)); 
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Stun",false));
}


FGameplayTagContainer UEnhancedGameplayAbility::GetActivationOwnedTags() const
{
	return ActivationOwnedTags; 
}

void UEnhancedGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	if (bActivateOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false); 
	}
}

void UEnhancedGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	
	// I don't implement anything here as this is more a reference of what I should call if I need to remove passive abilities
}

// we apply the cooldown based on the dynamic approach of only having the one Gameplay Effect
void UEnhancedGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// we first grab the cooldown effect of the ability and check that it is valid. 
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect(); 
	if (!CooldownGE) return; 
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(),GetAbilityLevel());
	if (!SpecHandle.IsValid()) return; 
	
	SpecHandle.Data->SetSetByCallerMagnitude(CooldownDurationTag, AbilityCooldown); 
	SpecHandle.Data.Get()->DynamicGrantedTags.AddTag(CooldownTag); 
	
	// we lastly want to apply the spec to the owner of the ability to activate the cooldown for them. 
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo,ActivationInfo, SpecHandle); // this will show a warning to due to not assigning the return value
}

// for this function, we are grabbing the tags it should be listening for when it is on cooldown
FGameplayTagContainer* UEnhancedGameplayAbility::GetCooldownTags() const
{
	// we create a temp value that will be getting all the tags gathered in one spot
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldowntags); 
	MutableTags->Reset(); 
	
	// we check if the cooldown tag we have is valid to add to the mutable tags
	if (CooldownTag.IsValid()) // errors here
	{
		MutableTags->AddTag(CooldownTag);
	}
	
	// next we check if the parent tags are a valid assignment and if so we add this to the mutable tags
	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		MutableTags->AppendTags(*ParentTags); 
	}
	return MutableTags; // we just return it at the end
}

// since we are doing dynamic allocation of cooldown tags, we don't use the standard Cooldown effect class due to the UE compiler refusing to compile blank effects
UGameplayEffect* UEnhancedGameplayAbility::GetCooldownGameplayEffect() const
{
	return DynamicCooldownEffect? DynamicCooldownEffect->GetDefaultObject<UGameplayEffect>():nullptr;
}

void UEnhancedGameplayAbility::IncrementLevel(const int32 IncreaseValue)
{
	const int32 NewLevel = GetAbilityLevel() + IncreaseValue; 
	if (NewLevel < AbilityMaxLevel)
	{
		SetAbilityLevel(NewLevel);
	}
}

void UEnhancedGameplayAbility::DecrementLevel(const int32 IncreaseValue)
{
	const int32 NewLevel = GetAbilityLevel() - IncreaseValue;
	if (NewLevel > AbilityMinLevel)
	{
		SetAbilityLevel(NewLevel);
	}
}

// since there is no matching set ability level, the equivalent has been made here and made blueprint call able
void UEnhancedGameplayAbility::SetAbilityLevel(const int32 NewAbilityLevel)
{
	ensure(CurrentActorInfo); 
	if (!CurrentActorInfo || !CurrentActorInfo->IsNetAuthority()) return;
	
	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo? CurrentActorInfo->AbilitySystemComponent.Get():nullptr;
	FGameplayAbilitySpec* Spec = AbilitySystemComponent? AbilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle): nullptr;
	
	if (!Spec)
	{
		ABILITY_LOG(Warning, TEXT("UEnhancedGameplayAbility::SetAbilityLevel. Invalid AbilitySpecHandle %s for Ability %s"), *CurrentSpecHandle.ToString(), *GetNameSafe(this)); 
		return;
	}
 	Spec->Level = NewAbilityLevel;
	AbilitySystemComponent->MarkAbilitySpecDirty(*Spec); 
}
