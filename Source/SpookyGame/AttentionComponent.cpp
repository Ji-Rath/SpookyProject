#include "AttentionComponent.h"
#include "Kismet/KismetMathLibrary.h"

UAttentionComponent::UAttentionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttentionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(LookingAt))
		HandleLookingAt(DeltaTime);
}

void UAttentionComponent::LookAt(AActor* LookAtActor, float Duration /*= 1.f*/)
{
	if (ensure(IsValid(LookAtActor)))
	{
		LookingAt = LookAtActor;
		GetOwner()->GetWorldTimerManager().SetTimer(LookAtTimer, this, &UAttentionComponent::ClearLookingAt, Duration, false);
	}
}

void UAttentionComponent::HandleLookingAt(float DeltaTime)
{
	AController* PlayerController = GetOwner<APawn>()->GetController();

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), LookingAt->GetActorLocation());
	FRotator ControlRot = UKismetMathLibrary::RInterpTo(PlayerController->GetControlRotation(), LookAtRot, DeltaTime, LookAtStrength);

	PlayerController->SetControlRotation(ControlRot);
}

void UAttentionComponent::ClearLookingAt()
{
	LookingAt = nullptr;
}

