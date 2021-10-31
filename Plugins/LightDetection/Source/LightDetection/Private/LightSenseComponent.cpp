#include "LightSenseComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/ShapeComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/LocalLightComponent.h"
#include "EngineUtils.h"

float ULightSenseComponent::CalculateLightLevel(UObject* WorldContextObject, const FVector& SurfacePos, const TArray<TSubclassOf<AActor>> ActorLights, TArray<FHitResult>& OutHits, TArray<FName> IgnoreLightTags, const float TraceDistance /*= 2000.f*/, const bool bUsePointLights /*= true*/, const bool bUseSpotlights /*= true*/, const bool bUseDirectionalLights /*= true*/)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	float LightVisibility = 0.f;

	// Loop through ALL light components in the level
	// TODO: Make this scalable
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;

		for (TSubclassOf<AActor> ActorClass : ActorLights)
		{
			// Make sure the actor we are checking for is one we want
			if (ActorClass && !Actor->IsA(ActorClass)) { continue; }

			// Get all light components from the actor to calculate
			TArray<ULightComponent*> Lights;
			Actor->GetComponents(Lights);

			FHitResult Hit;
			for (ULightComponent* Light : Lights)
			{
				LightVisibility += GetSingleLightLevel(WorldContextObject, Light, SurfacePos, {}, Hit, IgnoreLightTags, TraceDistance, ECC_Visibility, bUsePointLights, bUseSpotlights, bUseDirectionalLights);
				OutHits.Add(Hit);
			}
		}
	}

	return LightVisibility;
}

float ULightSenseComponent::GetSingleLightLevel(UObject* WorldContextObject, const ULightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, TArray<FName> IgnoreLightTags, const float TraceDistance /*= 2000.f*/, const ECollisionChannel Channel /*= ECC_Visibility*/, const bool bUsePointLights /*= true*/, const bool bUseSpotlights /*= true*/, const bool bUseDirectionalLights /*= true*/, const bool bDebug /*= false*/)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	
	float LightStrength = 0.f;
	// Ensure Light component is valid
	if (Light)
	{
		for (FName TagCheck : IgnoreLightTags)
		{
			if (Light->ComponentHasTag(TagCheck))
			{
				return 0.f;
			}
		}

		// Make sure light component is emitting light
		bool bIsOn = !FMath::IsNearlyEqual(Light->Intensity, 0.f);
		if (bIsOn)
		{
			// Attempt to loop through only relevant lights based on type and distance
			if (auto* LocalLight = Cast<ULocalLightComponent>(Light))
			{
				bool bWithinDistance = FVector::Distance(SurfacePos, Light->GetComponentLocation()) < LocalLight->AttenuationRadius;
				if (bWithinDistance)
				{
					if (auto* SpotLight = Cast<USpotLightComponent>(Light))
					{
						if (bUseSpotlights)
							LightStrength = GetSpotLightLevel(World, SpotLight, SurfacePos, ActorsToIgnore, OutHit, Channel);
					}
					else if (auto* PointLight = Cast<UPointLightComponent>(Light))
					{
						if (bUsePointLights && !PointLight->IsA(USpotLightComponent::StaticClass()))
							LightStrength = GetPointLightLevel(World, PointLight, SurfacePos, ActorsToIgnore, OutHit, Channel);
					}
				}
			}
			else if (auto* DirectionalLight = Cast<UDirectionalLightComponent>(Light))
			{
				if (bUseDirectionalLights)
					LightStrength = GetDirectionalLightLevel(World, DirectionalLight, SurfacePos, ActorsToIgnore, OutHit, TraceDistance, Channel);
			}
		}
	}

	if (bDebug)
	{
		if (OutHit.GetActor())
		{
			DrawDebugLine(World, OutHit.TraceStart, OutHit.TraceEnd, FColor::Green, false, 1.f, 0, 2.f);
			DrawDebugPoint(World, OutHit.ImpactPoint, 5.f, FColor::Red, false, 1.f);
		}
		else
		{
			DrawDebugLine(World, OutHit.TraceStart, OutHit.TraceEnd, FColor::Red, false, 1.f, 0, 1.f);
		}
	}

	return LightStrength;
}

float ULightSenseComponent::GetCurrentLightLevel() const
{
	return LightLevel;
}

ULightSenseComponent::ULightSenseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.5f;

	LightActors.Add(AActor::StaticClass());
}

void ULightSenseComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<FHitResult> Hit;
	LightLevel = CalculateLightLevel(GetWorld(), GetOwner()->GetActorLocation(), LightActors, Hit, IgnoreLightComponentTags, LightTraceDistance, bUsePointLights, bUseSpotlights, bUseDirectionalLights);
}

void ULightSenseComponent::BeginPlay()
{
	Super::BeginPlay();
}

float ULightSenseComponent::GetSpotLightLevel(const UWorld* World, const USpotLightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, const ECollisionChannel Channel /*= ECC_Visibility*/) 
{
	float LightVisibility = 0.f;
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
				LightVisibility = GetPointLightLevel(World, Light, SurfacePos, ActorsToIgnore, OutHit, Channel);
		}
	}
	return LightVisibility;
}

float ULightSenseComponent::GetPointLightLevel(const UWorld* World, const UPointLightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, const ECollisionChannel Channel /*= ECC_Visibility*/)
{
	float LightVisibility = 0.f;
	
	// Reduce visible range based on intensity of light
	float UnitFactor = Light->GetUnitsConversionFactor(Light->IntensityUnits, ELightUnits::Candelas);
	float IntensityFactor = (Light->Intensity * UnitFactor) / 20.f;

	float VisibleRange = Light->AttenuationRadius * IntensityFactor;
	float ClearVisibleRange = VisibleRange / 3;

	if (ensure(Light))
	{
		FVector LightDirection = UKismetMathLibrary::GetForwardVector(Light->GetComponentRotation());
		FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
		CollisionParams.AddIgnoredActors(ActorsToIgnore);
		bool bTraced = World->LineTraceSingleByChannel(OutHit, Light->GetComponentLocation(), SurfacePos, Channel, CollisionParams);
		
		// Determine light level based on distance to light
		if (!bTraced)
		{
			float Distance = FVector::Distance(SurfacePos, Light->GetComponentLocation());
			LightVisibility = 1-UKismetMathLibrary::NormalizeToRange(Distance, ClearVisibleRange, VisibleRange);
		}
	}
	return FMath::Clamp(LightVisibility, 0.f, 1.f);
}

float ULightSenseComponent::GetDirectionalLightLevel(const UWorld* World, const UDirectionalLightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, const float TraceDistance /*= 2000.f*/, const ECollisionChannel Channel /*= ECC_Visibility*/)
{
	bool bTraced = false;
	if (ensure(Light))
	{
		FVector LightDirection = UKismetMathLibrary::GetForwardVector(Light->GetComponentRotation()) * -1;
		FVector EndPos = SurfacePos + (LightDirection * TraceDistance);
		FCollisionQueryParams CollisionParams = FCollisionQueryParams::DefaultQueryParam;
		CollisionParams.AddIgnoredActors(ActorsToIgnore);

		bTraced = World->LineTraceSingleByChannel(OutHit, SurfacePos, EndPos, Channel, CollisionParams);
	}
	return bTraced ? 0.f : 1.f;
}