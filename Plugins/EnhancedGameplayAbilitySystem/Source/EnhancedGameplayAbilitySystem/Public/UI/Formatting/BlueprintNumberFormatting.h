
#pragma once

#include "CoreMinimal.h"
#include "BlueprintNumberFormatting.generated.h"

USTRUCT(BlueprintType)
struct FBlueprintNumberFormat
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting")
	bool bAlwaysSign = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting")
	bool bUseGrouping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting")
	bool IndicateNearlyInteger = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting")
	TEnumAsByte<ERoundingMode> RoundingMode = ERoundingMode::FromZero; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting", meta = (ClampMin = 0, UIMin = 0))
	int32 MinimumIntegralDigits = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting", meta = (ClampMin = 0, UIMin = 0))
	int32 MaximumIntegralDigits = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting", meta = (ClampMin = 0, UIMin = 0))
	int32 MinimumFractionalDigits = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting", meta = (ClampMin = 0, UIMin = 0))
	int32 MaximumFractionalDigits = 0;
	
	FNumberFormattingOptions ToNativeOptions() const
	{
		FNumberFormattingOptions Native; 
		Native.SetAlwaysSign(bAlwaysSign);
		Native.SetUseGrouping(bUseGrouping);
		Native.SetIndicateNearlyInteger(IndicateNearlyInteger);
		Native.SetRoundingMode(RoundingMode);
		Native.SetMinimumFractionalDigits(MinimumFractionalDigits);
		Native.SetMaximumFractionalDigits(MaximumFractionalDigits);
		Native.SetMinimumIntegralDigits(MinimumIntegralDigits);
		Native.SetMaximumIntegralDigits(MaximumIntegralDigits);
		return Native;
	}
};

UCLASS()

class ENHANCEDGAMEPLAYABILITYSYSTEM_API UTextFormattingHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Formatting")
	static FText FormatFloatWithCustomOptions(float Value, const FBlueprintNumberFormat& Options); 
};