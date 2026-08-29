#include "UI/Formatting/BlueprintNumberFormatting.h"

FText UTextFormattingHelper::FormatFloatWithCustomOptions(float Value, const FBlueprintNumberFormat& Options)
{
	FNumberFormattingOptions NativeOptions = Options.ToNativeOptions(); 
	return FText::AsNumber(Value, &NativeOptions);
}
