#include "MoreMovement.h"

DEFINE_LOG_CATEGORY(MoreMovement);

#define LOCTEXT_NAMESPACE "FMoreMovement"

void FMoreMovement::StartupModule()
{
	UE_LOG(MoreMovement, Warning, TEXT("MoreMovement module has been loaded"));
}

void FMoreMovement::ShutdownModule()
{
	UE_LOG(MoreMovement, Warning, TEXT("MoreMovement module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMoreMovement, MoreMovement)