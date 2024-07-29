#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GenericPlatform/GenericWindow.h"
#include "SettingsWidget.generated.h"

UCLASS()
class SPOOKYGAME_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Convert String quality to integer
	 * @param Quality Low, Med, High, Epic
	 * @return int Quality level from 0-3
	*/
	UFUNCTION(BlueprintCallable)
	int GetQuality(const FString& QualityString);

	/**
	 * Convert fullscreen mode string to EWindowMode enum
	 * @param FullscreenString Windowed, Fullscreen, Borderless
	 * @return EWindowMode
	*/
	UFUNCTION(BlueprintCallable)
	EWindowMode::Type GetFullscreenMode(const FString& FullscreenString);
};

