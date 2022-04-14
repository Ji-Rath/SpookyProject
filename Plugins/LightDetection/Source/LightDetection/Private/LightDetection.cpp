#include "LightDetection.h"

DEFINE_LOG_CATEGORY(LightDetection);

#define LOCTEXT_NAMESPACE "FLightDetection"

void FLightDetection::StartupModule()
{
	UE_LOG(LightDetection, Warning, TEXT("LightDetection module has been loaded"));
}

void FLightDetection::ShutdownModule()
{
	UE_LOG(LightDetection, Warning, TEXT("LightDetection module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLightDetection, LightDetection)