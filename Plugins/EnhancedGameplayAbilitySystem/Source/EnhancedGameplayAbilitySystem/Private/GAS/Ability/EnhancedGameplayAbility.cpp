// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/EnhancedGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UEnhancedGameplayAbility::UEnhancedGameplayAbility():ActivationPolicy(EEnhancedActivationPolicy::OnInputTriggered),bActivateOnGranted(false)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	FGameplayTagContainer AbilityContainer;
	AbilityContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GameplayAbility"))); 
	SetAssetTags(AbilityContainer); 
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead",false)); 
	
}

ACharacter* UEnhancedGameplayAbility::GetCharacterFromActorInfo() const
{
	return(CurrentActorInfo ? Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr); 
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

void UEnhancedGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	// we first grab the cooldown effect of the ability and check that it is valid. 
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect(); 
	if (!CooldownGE) return; 
	
	// from this we grab the spec handle and check if it is valid. 
	// we do this as it means that we can alter values based on what we have given the ability. Mainly the cooldown duration and tags
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(),GetAbilityLevel());
	if (!SpecHandle.IsValid()) return; 
	
	// we will then set the cooldown duration of the ability based on the cooldown duration tag and the float value
	SpecHandle.Data->SetSetByCallerMagnitude(CooldownDurationTag, AbilityCooldown); 
	
	// lastly we tell it, this is the tag that we will be using to block the ability when on cooldown
	SpecHandle.Data.Get()->DynamicGrantedTags.AddTag(CooldownTag); 
	
	// we lastly want to apply the spec to the owner of the ability to activate the cooldown for them. 
	// rider will flag this as an error as we aren't assigning the return value, so we just put in the comment below to tell it to shut up
	// ReSharper disable once CppExpressionWithoutSideEffects
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo,ActivationInfo, SpecHandle); 
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

UGameplayEffect* UEnhancedGameplayAbility::GetCooldownGameplayEffect() const
{
	return DynamicCooldownEffect? DynamicCooldownEffect->GetDefaultObject<UGameplayEffect>():nullptr;
}
