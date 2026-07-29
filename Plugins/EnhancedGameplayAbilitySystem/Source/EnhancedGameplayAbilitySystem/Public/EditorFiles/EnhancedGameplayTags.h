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
	
	// we would also do ability ones here if we wanted to

	// blocking movement tags
	FGameplayTag Gameplay_Death;
	FGameplayTag Gameplay_Aim;
	FGameplayTag Gameplay_Stun;
	
protected:
	
	virtual void AddAllTags(UGameplayTagsManager& Manager); 
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagValue); 
	
private: 
	static FEnhancedGameplayTags GameplayTags; 
};
