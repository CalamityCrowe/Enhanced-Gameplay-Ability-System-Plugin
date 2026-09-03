// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/EnhancedAttributeSet.h"

// engine 
#include "GameplayEffectExtension.h"
//plugin 
#include "Characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/EnhancedPlayerController.h"


UEnhancedAttributeSet::UEnhancedAttributeSet():Health(100.0f), MaxHealth(100.f), Damage(0.0f), Shield(100), MaxShield(100)
,XP(0.f), MaxXP(3.f),  XPGained(0.0f), XPBounty(5), Level(1.0f)
{
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"), false); 
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"), false); 
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"), false); 
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"), false); 
}

void UEnhancedAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// might do something here to adjust the current health to increase / decrease by the same amount
	}
	else if (Attribute == GetMaxShieldAttribute())
	{
		
	}
	else if(Attribute == GetXPAttribute() || Attribute == GetMaxXPAttribute())
	{
		NewValue = FMath::Max(NewValue, 0); // ensures that aren't putting the XP into a negative value
	}
	else if(Attribute == GetLevelAttribute())
	{
		NewValue = FMath::Clamp(NewValue,1, 99);
	}
	
}

void UEnhancedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	// we first grab a few pieces from the data to help make some of the later contexts, like source and target actors
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext(); 
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent(); 
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags(); 
	FGameplayTagContainer SpecAssetTags; 
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags); 
	
	// we will then check if there is a valid target actor from the data along with if the ability actor info is valid
	AActor* TargetActor = nullptr; 
	AController* TargetController = nullptr; 
	ACharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacterBase>(TargetActor);
	}
	
	
	// same principle as the target actor, but we have an additional check if the source we grabbed earlier is valid
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ACharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceActor && !SourceController)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}
		
		if (SourceController)
		{
			SourceCharacter = Cast<ACharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast <ACharacterBase>(SourceActor);
		}
		
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}
	
#pragma region Handling Attributes
	
	/*
	 * Here is we will be handling all the attributes that are normally associated with characters in any games
	 * for this, the main focus is how do we actually apply damage to the character, what order it affects these attributes and what do we do
	 * when the character actually dies. 
	 * 
	 * along side this, there is a basic level up system that will decide if the character is ready to level up.
	 * Outside of these it is the generic clamping of attributes to ensure they can't go out of bounds
	 */
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// we grab the damage to store in a temp location, then we reset it, to prevent different damages from applying more than they need to 
		float LocalDamage = GetDamage(); 
		const float DisplayDamage = LocalDamage; // this is used for displaying damage numbers when they are hit
		SetDamage(0.0f);
		
		// we will check if there is a damage value that is greater than 0 so we don't process irrelevant hit react
		if (LocalDamage > 0.0f)
		{
			// we will check that the target is actually
			bool bWasAlive = true; 
			if (TargetCharacter)
			{
				bWasAlive = TargetCharacter->IsAlive();
			}
			float LocalShield = GetShield(); 
			
			if (LocalShield > 0.0f)
			{
				SetShield(FMath::Clamp(LocalShield - LocalDamage, 0.0f, GetMaxShield()));
				LocalDamage = LocalDamage - LocalShield;
				if (LocalDamage > 0.0f)
				{
					const float LocalHealth = GetHealth() - LocalDamage; 
					SetHealth(FMath::Clamp(LocalHealth, 0.0f, GetMaxHealth()));
				}
			}
			else
			{
				// next we will apply the damage to the health and clamp it so we don't go into the negatives
				const float LocalHealth = GetHealth() - LocalDamage; 
				SetHealth(FMath::Clamp(LocalHealth, 0.0f, GetMaxHealth()));
			}

			
			// we will check if the target character is valid still and still alive
			if (TargetCharacter && bWasAlive)
			{
				// if so we will get the hit location from the effect and then use it do decide how the target should do the hit react from that  
				if (const FHitResult* Hit = Context.GetHitResult())
				{
					switch(EHitReactDirection HitDirection = TargetCharacter->GetHitReactDirection(Hit->Location))
					{
					case EHitReactDirection::Left: 
						TargetCharacter->PlayHitReact(HitDirectionLeftTag); 
						break; 
					case EHitReactDirection::Front:
						TargetCharacter->PlayHitReact(HitDirectionFrontTag);
						break; 
					case EHitReactDirection::Right:
						TargetCharacter->PlayHitReact(HitDirectionRightTag);
						break; 
					case EHitReactDirection::Back:
						TargetCharacter->PlayHitReact(HitDirectionBackTag);
						break; 
					default:
						break;
					}
				}
				else // if there is no relevant hit information and we still took damage, then we still do the forward react to signal to the player we took damage
				{
					TargetCharacter->PlayHitReact(HitDirectionFrontTag); 
				}
				
				if (SourceActor != TargetActor)
				{
					if (AEnhancedPlayerController* PC = Cast<AEnhancedPlayerController>(SourceController))
					{
						PC->ShowDamageNumber(DisplayDamage,TargetActor); 
					}
				}	
				if (!TargetCharacter->IsAlive())
				{
					if (SourceController != TargetController)
					{
						// took this from traneks documentations
						
						// we are creating a blank gameplay effect to use as a template to populate with modifiers
						// the transient package is used here as it is a temporary file that we will be discarding once we are done with it
						UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName("Bounty"));
						GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant; 
						
						// we grab how many modifiers we have in the effect(should be 0 here) and add a fixed value to it for the number of moddifers
						// in this case it will only be one as we are only adding to the XPGained attribute
						const int32 Idx = GEBounty->Modifiers.Num(); 
 						GEBounty->Modifiers.SetNum(Idx +1);
						
						// from here we can make a modifier info that will setup how we want it to behave and where it is getting the values from
						// here he are grabbing the XP bounty (this will be from the target) for the value and setting it to additive
						// to be applied to the XPGained attribute
						FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
						InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
						InfoXP.ModifierOp = EGameplayModOp::Additive; 
						InfoXP.Attribute = UEnhancedAttributeSet::GetXPGainedAttribute(); 
						
						
						Source->ApplyGameplayEffectToSelf(GEBounty,1.0f, Source->MakeEffectContext());  // lastly, we will be applying this to the source
					}
				} 
			}
		}
	}
	// checks if the current attribute that is getting changed is XPGained
	else if (Data.EvaluatedData.Attribute == GetXPGainedAttribute())
	{
		// grab the gained amount and the current xp locally so we can make changes to the values
		const float LocalXPGained = GetXPGained(); 
		const float LocalXP = GetXP(); 
		SetXPGained(0.0f);  // resets the XP gained so it doesn't accumulate over, similar concept to damage

		SetXP(LocalXP + LocalXPGained);
		if ((LocalXP + LocalXPGained) > GetMaxXP()) // Checks if the new XP is greater than the max XP and if so, we trigger the level up function
		{
			TriggerLevelUp(); 
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth())); 
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield())); 
	}
	// will do something later with this
#pragma endregion 
}

#pragma region  Rep notifications
void UEnhancedAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// helper functions to define how the notify works for the attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, Health, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, Shield, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, MaxShield, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, XP,COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, MaxXP,COND_None, REPNOTIFY_Always); 
	DOREPLIFETIME_CONDITION_NOTIFY(UEnhancedAttributeSet, Level,COND_None, REPNOTIFY_Always); 
}

// this will be what we do for the on rep notifies, which will just call the helper functions in the base class for attributes 
void UEnhancedAttributeSet::OnRep_Health(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, Health, OldData);
}

void UEnhancedAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, MaxHealth, OldData);
}

void UEnhancedAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, Shield, OldData);
}

void UEnhancedAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, MaxShield, OldData);
}

void UEnhancedAttributeSet::OnRep_XP(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, XP, OldData);
}

void UEnhancedAttributeSet::OnRep_MaxXP(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, MaxXP, OldData);
}

void UEnhancedAttributeSet::OnRep_Level(const FGameplayAttributeData& OldData)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEnhancedAttributeSet, Level, OldData);
}
#pragma endregion

void UEnhancedAttributeSet::TriggerLevelUp()
{
	// we grab the XP at the start,subtract the MaxXP to get the overflow amount and set the XP to the overflow 
	const float LocalXP = GetXP(); 
	const float Difference = LocalXP - GetMaxXP();
	SetXP(Difference);
	SetLevel(FMath::RoundToInt(GetLevel())+1); // we increase the current characters level by 1
	
	// we can grab the curve from the source object by defining the MaxXP curve in there?
	// means passing in the source object as a reference, so we can grab the XP curve
	
	if (GetXP() >= GetMaxXP()) TriggerLevelUp(); 
}
