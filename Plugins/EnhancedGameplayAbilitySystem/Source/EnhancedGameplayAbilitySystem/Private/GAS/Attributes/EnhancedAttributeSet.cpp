// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attributes/EnhancedAttributeSet.h"

// engine 
#include "GameplayEffectExtension.h"
//plugin 
#include "Characters/CharacterBase.h"


UEnhancedAttributeSet::UEnhancedAttributeSet():Health(100.0f), MaxHealth(100.f), Damage(0.0f)
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
		TargetCharacter = Cast<ACharacterBase>(TargetCharacter);
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
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// we grab the damage to store in a temp location, then we reset it, to prevent different damages from applying more than they need to 
		const float LocalDamage = GetDamage(); 
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
			// next we will apply the damage to the health and clamp it so we don't go into the negatives
			const float NewHealth = GetHealth() - LocalDamage; 
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			
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
					// we can do some damage numbers and effects here if we really wanted to, ideally only on the enemies.
					// so what we can do is grab the 
				}	
			}
			
			
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth()); 
	}
	// will do something later with this
	
}
