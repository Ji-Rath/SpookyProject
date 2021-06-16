#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightSenseComponent.generated.h"

class UPointLightComponent;
class USpotLightComponent;
class UDirectionalLightComponent;
class ULocalLightComponent;
class UShapeComponent;
class ALight;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPOOKYGAME_API ULightSenseComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	/**
	 * Determine light level of surface based on given light components
	 * @param Lights Array of light components
	 * @param SurfacePos Surface to get light level of
	 * @return float LightLevel, 0 - Not visible,  > 1 - Fully visible
	*/
	UFUNCTION(BlueprintCallable)
	float CalculateLightLevel(const FVector& SurfacePos) const;

	/**
	 * Calculate the light level of the surface for one light component
	 * @param Light component
	 * @param SurfacePos
	 * @return float Light level
	*/
	UFUNCTION()
	float GetSingleLightLevel(ULightComponent* Light, const FVector& SurfacePos) const;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginPlay() override;

	/**
	 * Gets the current light level of the actor
	 * @return float
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentLightLevel() const;

	ULightSenseComponent();

private:
	
	UFUNCTION()
	float GetSpotLightLevel(USpotLightComponent* Light, const FVector& SurfacePos) const;

	UFUNCTION()
	float GetPointLightLevel(UPointLightComponent* Light, const FVector& SurfacePos) const;

	UFUNCTION()
	float GetDirectionalLightLevel(UDirectionalLightComponent* Light, const FVector& SurfacePos) const;

	UPROPERTY()
	float LightLevel = 0.f;

	/** Distance to trace for blocking objects while calculating for directional light level */
	UPROPERTY(EditDefaultsOnly)
	float LightTraceDistance = 2000.f;
};
