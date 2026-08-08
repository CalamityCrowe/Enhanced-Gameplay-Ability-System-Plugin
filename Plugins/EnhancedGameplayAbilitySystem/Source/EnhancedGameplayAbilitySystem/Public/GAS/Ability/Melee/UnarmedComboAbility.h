// 

#pragma once

#include "CoreMinimal.h"
#include "BaseUnarmedAbility.h"
#include "UnarmedComboAbility.generated.h"

/**
 * similar to the class this inherits off of, this is extended functionality to handle basic combo chain. I.E: press square 4 times
 * 
 * this would be done similarly for a melee weapon with the exception,  we would handle the hit scans on the weapon manager 
 * 
 */
UCLASS(Abstract, HideDropdown)
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UUnarmedComboAbility : public UBaseUnarmedAbility
{
	GENERATED_BODY()
	
public: 
	
	UUnarmedComboAbility(); 
	
	virtual void MontageStarted() override;
	
protected: 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Abilities|Events", meta = (ToolTips = "This will be the tag we will be listening for from the montage playing to indicate that we can recieve input to continue the combo"))
	FGameplayTag ComboStartTag; 
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Abilities|Events", meta = (ToolTips = "This will be the tag we will be listening for from the montage playing to indicate we can no longer accept combo inputs and potentially end the ability if no inputs were recieved"))
	FGameplayTag ComboEndTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Abilities|Events" ,meta = (ToolTips = "This will be the tag we will listen for getting sent to the actor performing the ability from their input"))
	FGameplayTag ComboInputTag;
	
private:
	UFUNCTION()
	virtual void ContinueComboStartEvent(FGameplayEventData Payload);
	UFUNCTION()
	virtual void ContinueComboEndEvent(FGameplayEventData Payload);
	UFUNCTION()
	virtual void InputReceivedEvent(FGameplayEventData Payload); 
	
	int ComboCount; 
	
	bool bIsWithinComboWindow;
	bool bReceivedInputAtRightTime;
	
};
