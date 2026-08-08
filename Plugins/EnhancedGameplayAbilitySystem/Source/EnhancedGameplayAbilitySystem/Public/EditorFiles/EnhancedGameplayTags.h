#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagContainer;

struct ENHANCEDGAMEPLAYABILITYSYSTEM_API FEnhancedGameplayTags
{
public:
	virtual ~FEnhancedGameplayTags() = default;
	// 
	static const FEnhancedGameplayTags& Get(){return GameplayTags; }
	static void InitializeNativeTags(); 
	
public: 
	// Native inputs
	FGameplayTag Input_Move; 
	FGameplayTag Input_Aim;
	FGameplayTag Input_Jump;
	
	
	// gameplay related Tags
	FGameplayTag Gameplay_Death;
	FGameplayTag Gameplay_Aim;
	FGameplayTag Gameplay_Stun;
	
	FGameplayTag Gameplay_Ability;
	
	// Event based tags
	FGameplayTag Event_AbilitiesChanged;
	FGameplayTag Event_MontageTriggered;
	FGameplayTag Event_ComboStart;
	FGameplayTag Event_ComboEnd;
	FGameplayTag Event_ComboInput;
	
	FGameplayTag Event_HitScanStart;
	FGameplayTag Event_HitScanEnd;
	
	
	
protected:
	
	virtual void AddAllTags(UGameplayTagsManager& Manager); 
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagValue); 
	
private: 
	static FEnhancedGameplayTags GameplayTags; 
	
	FGameplayTag Data_Damage; 
	FGameplayTag Data_Heal;
};
