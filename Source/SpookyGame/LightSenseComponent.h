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
	float CalculateLightLevel(TArray<ULightComponent*> Lights, const FVector& SurfacePos) const;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginPlay() override;

	/**
	 * Loop through ALL light components in the level and return them
	 * @param Lights OUT light array
	 * @return void
	*/
	UFUNCTION()
	void FindLightComponents(TArray<ULightComponent*>& OutLightArray);

	/**
	 * Gets the current light level of the actor
	 * @return float
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetLightLevel() const;

	ULightSenseComponent();
	
	/** Contains all light components in the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ULightComponent*> LightArray;

private:
	
	UFUNCTION()
	float GetSpotLightLevel(USpotLightComponent* Light, const FVector& SurfacePos) const;

	UFUNCTION()
	float GetPointLightLevel(UPointLightComponent* Light, const FVector& SurfacePos) const;

	UFUNCTION()
	float GetDirectionalLightLevel(UDirectionalLightComponent* Light, const FVector& SurfacePos) const;

	UPROPERTY()
	float LightLevel = 0.f;
};
