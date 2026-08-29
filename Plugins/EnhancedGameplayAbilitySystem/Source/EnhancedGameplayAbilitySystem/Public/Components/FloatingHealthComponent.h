// 

#pragma once

#include "CoreMinimal.h"
#include "LookAtWidgetComponent.h"
#include "FloatingHealthComponent.generated.h"


class ACharacterBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENHANCEDGAMEPLAYABILITYSYSTEM_API UFloatingHealthComponent : public ULookAtWidgetComponent
{
	GENERATED_BODY()

public:
	UFloatingHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
private: 
	UPROPERTY()
	TWeakObjectPtr<ACharacterBase> OwningCharacterRef;
};
