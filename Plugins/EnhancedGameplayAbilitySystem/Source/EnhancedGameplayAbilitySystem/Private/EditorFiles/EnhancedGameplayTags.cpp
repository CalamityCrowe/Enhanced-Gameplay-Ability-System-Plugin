#include "EditorFiles/EnhancedGameplayTags.h"
#include "GameplayTagsManager.h"

FEnhancedGameplayTags FEnhancedGameplayTags::GameplayTags;

void FEnhancedGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get(); 
	GameplayTags.AddAllTags(Manager); 
	Manager.DoneAddingNativeTags(); 
}

void FEnhancedGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	FGameplayTag Temp; 
	
	// Native Inputs
	AddTag(Input_Move, "NativeInput.Move", "Movement Inputs"); 
	AddTag(Input_Aim, "NativeInput.Aim", "Aim Inputs");
	AddTag(Input_Jump, "NativeInput.Jump", "Jump Inputs");
	
	AddTag(Gameplay_Death, "State.Dead", "Dead");
	AddTag(Gameplay_Stun, "State.Stun", "Stun");
	AddTag(Gameplay_Aim, "State.Aim", "Aim"); // not sure if I want this as a state yet or an ability 
	
	// will look at loading these in via a file instead later for the non input tags
	AddTag(Gameplay_Ability, "GameplayAbility", "Abilities");
	
	AddTag(Event_AbilitiesChanged,"Event.Abilities.Changed", "Abilities changed event");
	AddTag(Event_MontageTriggered, "Event.Montage.Triggered", "Montage triggered");
	AddTag(Event_ComboStart, "Event.Combo.Start", "Combo Start"); 
	AddTag(Event_ComboEnd, "Event.Combo.End", "Combo End");
	AddTag(Event_HitScanStart, "Event.Hitscan.Start", "Hit scan start");
	AddTag(Event_HitScanEnd, "Event.Hitscan.End", "Hit scan end");

	AddTag(Temp, "Data.Cooldown","Cooldown"); 
	AddTag(Data_Damage, "Data.Damage", "Damage");
	AddTag(Data_Heal, "Data.Heal", "Heal");
	
		
	AddTag(Temp, "Status.Buff.Shield", "Shield Status"); 
}

void FEnhancedGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagValue)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native )"))+FString(TagValue));
}
