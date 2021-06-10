#include "SettingsWidget.h"



int USettingsWidget::GetQuality(const FString& QualityString)
{
	if (QualityString == "Low")
	{
		return 0;
	}
	else if (QualityString == "Med")
	{
		return 1;
	}
	else if (QualityString == "High")
	{
		return 2;
	}
	else if (QualityString == "Epic")
	{
		return 3;
	}
	return 3;
}

EWindowMode::Type USettingsWidget::GetFullscreenMode(const FString& FullscreenString)
{
	if (FullscreenString == "Windowed")
	{
		return EWindowMode::Windowed;
	}
	else if (FullscreenString == "Borderless")
	{
		return EWindowMode::WindowedFullscreen;
	}
	else if (FullscreenString == "Fullscreen")
	{
		return EWindowMode::Fullscreen;
	}

	return EWindowMode::Fullscreen;
}
