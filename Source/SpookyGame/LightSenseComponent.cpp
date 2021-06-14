#include "LightSenseComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/ShapeComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/LocalLightComponent.h"

float ULightSenseComponent::GetLightLevel(TArray<ULightComponent*> Lights, const FVector& SurfacePos) const
{
	float LightLevel = 0.f;

	for (ULightComponent* Light : Lights)
	{
		bool bWithinDistance = FVector::Distance(SurfacePos, Light->GetComponentLocation()) < TraceDistance;
		if (bWithinDistance || Cast<UDirectionalLightComponent>(Light))
		{
			if (auto* SpotLight = Cast<USpotLightComponent>(Light))
			{
				LightLevel += GetSpotLightLevel(SpotLight, SurfacePos);
			}
			else if (auto* PointLight = Cast<UPointLightComponent>(Light))
			{
				LightLevel += GetPointLightLevel(PointLight, SurfacePos);
			}
			else if (auto* DirectionalLight = Cast<UDirectionalLightComponent>(Light))
			{
				LightLevel += GetDirectionalLightLevel(DirectionalLight, SurfacePos);
			}
		}
	}

	return LightLevel;
}

float ULightSenseComponent::GetVisibility() const
{
	return LightVisibility;
}

ULightSenseComponent::ULightSenseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.5f;
}

void ULightSenseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LightVisibility = GetLightLevel(LightArray, GetOwner()->GetActorLocation());
}

void ULightSenseComponent::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* MyWorld = GetWorld();
	// change UObject to the type of UObject you're after
	for (TObjectIterator<ULightComponent> ObjectItr; ObjectItr; ++ObjectItr)
	{
		// skip if this object is not associated with our current game world
		if (ObjectItr->GetWorld() != MyWorld)
		{
			continue;
		}

		ULightComponent* Object = *ObjectItr;

		if (Object->GetOwner() != GetOwner())
			LightArray.Add(Object);
	}
}

float ULightSenseComponent::GetSpotLightLevel(USpotLightComponent* Light, const FVector& SurfacePos) const 
{
	float LightLevel = 0.f;
	if (ensure(Light))
	{
		float LightRange = Light->AttenuationRadius;
		float LightAngle = Light->OuterConeAngle;
	
		float Distance = FVector::Distance(Light->GetComponentLocation(), SurfacePos);

		// Only determine visibility if within range
		if (Distance < LightRange)
		{
			// Determine angle from light to surface
			FVector LightDirection = Light->GetOwner()->GetActorForwardVector();
			FVector Point = SurfacePos - Light->GetComponentLocation();
			Point.Normalize();

			float Dot = FVector::DotProduct(Point, LightDirection);
			float AngleToSurface = UKismetMathLibrary::DegAcos(Dot);

			// Determine whether the surface is in light based on line trace
			if (AngleToSurface < LightAngle)
				LightLevel = GetPointLightLevel(Light, SurfacePos);
		}
	}
	return LightLevel;
}

float ULightSenseComponent::GetPointLightLevel(UPointLightComponent* Light, const FVector& SurfacePos) const
{
	float LightLevel = 0.f;
	float AttenRadius = Light->AttenuationRadius;
	float VisibleRange = AttenRadius / 2;

	if (ensure(Light))
	{
		FHitResult Hit;
		FVector LightDirection = UKismetMathLibrary::GetForwardVector(Light->GetComponentRotation());

		FVector EndPos = SurfacePos + (LightDirection * TraceDistance);

		bool bTraced = GetWorld()->LineTraceSingleByChannel(Hit, SurfacePos, Light->GetComponentLocation(), ECC_Visibility);
		

		// Determine light level based on distance to light
		if (!bTraced)
		{
			float Distance = FVector::Distance(SurfacePos, Light->GetComponentLocation());
			LightLevel = 1-UKismetMathLibrary::NormalizeToRange(Distance, VisibleRange, AttenRadius);
			DrawDebugLine(GetWorld(), SurfacePos, Light->GetComponentLocation(), FColor::Red, false, 1.f, 0, 1.f);
		}
		else
		{
			DrawDebugLine(GetWorld(), SurfacePos, Light->GetComponentLocation(), FColor::Green, false, 1.f, 0, 1.f);
		}
	}
	return FMath::Clamp(LightLevel, 0.f, 1.f);
}

float ULightSenseComponent::GetDirectionalLightLevel(UDirectionalLightComponent* Light, const FVector& SurfacePos) const
{
	bool bTraced = false;
	if (ensure(Light))
	{
		FHitResult Hit;
		FVector LightDirection = UKismetMathLibrary::GetForwardVector(Light->GetComponentRotation()) * -1;
		FVector EndPos = SurfacePos + (LightDirection * TraceDistance);

		bTraced = GetWorld()->LineTraceSingleByChannel(Hit, SurfacePos, EndPos, ECC_Visibility);
		if (bTraced)
		{
			DrawDebugLine(GetWorld(), SurfacePos, EndPos, FColor::Green, false, 1.f, 0, 1.f);
		}
		else
		{
			DrawDebugLine(GetWorld(), SurfacePos, EndPos, FColor::Red, false, 1.f, 0, 1.f);
		}
	}
	return bTraced ? 0.f : 1.f;
}