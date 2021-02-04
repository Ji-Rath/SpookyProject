#include "AttentionComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UAttentionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(LookingAt))
		HandleLookingAt(DeltaTime);
}

void UAttentionComponent::ForceLookAt(AActor* LookAt, float Duration /*= 1.f*/)
{
	if (ensure(IsValid(LookAt)))
	{
		LookingAt = LookAt;
		GetOwner()->GetWorldTimerManager().SetTimer(LookAtTimer, this, &UAttentionComponent::ClearLookingAt, Duration, false);
	}
}

void UAttentionComponent::HandleLookingAt(float DeltaTime)
{
	AController* PlayerController = GetOwner<APawn>()->GetController();

	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), LookingAt->GetActorLocation());
	FRotator ControlRot = UKismetMathLibrary::RInterpTo(PlayerController->GetControlRotation(), LookAtRot, DeltaTime, LookAtSpeed);

	PlayerController->SetControlRotation(ControlRot);
}

void UAttentionComponent::ClearLookingAt()
{
	LookingAt = nullptr;
}
