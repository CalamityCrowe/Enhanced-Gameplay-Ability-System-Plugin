// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EnhancedAbilitySet.h"

// engine
#include "GameplayAbilitySpecHandle.h"
// enhanced gameplay ability plugin
#include "GAS/EnhancedAbilitySystemComponent.h"
#include "GAS/Ability/EnhancedGameplayAbility.h"


void FAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		GrantedAbilitySpecHandles.Add(Handle); 
	}
}

void UEnhancedAbilitySet::GiveToAbilitySystem(UEnhancedAbilitySystemComponent* InASC, FAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(InASC);
	
	if (!InASC->IsOwnerActorAuthoritative())
	{
		// we only want to give abilities on the server, even though this game is single player only for now it's a good practice to accomadate this for future projects
		return; 
	}
	
	
	// we loop through all the abilities that are going to be granted to the ASC
	for (const FAbilitySet_GameplayAbility& AbilityInfo: GrantedAbilities)
	{
		const FAbilitySet_GameplayAbility& AbilityToGrant = AbilityInfo; 
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities on Ability Set [%s] is not valid."), *GetNameSafe(this));
			continue; 
		}
		
		// we get the default object of the ability to create a spec from it
		UEnhancedGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UEnhancedGameplayAbility>();
		
		// we create a spec from the actual ability and assign the source object and inputs to it
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel); 
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.AbilityInputTag); 
		
		// after the spec is created we then create a handle for it by giving the ability directly to the ASC
		const FGameplayAbilitySpecHandle GrantedHandle = InASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(GrantedHandle);
		}
	}
}
