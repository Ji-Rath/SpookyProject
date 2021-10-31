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
class LIGHTDETECTION_API ULightSenseComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	/**
	 * Determine light level of surface based on given light components
	 * @param Lights Array of light components
	 * @param SurfacePos Surface to get light level of
	 * @return float LightLevel, 0 - Not visible,  > 1 - Fully visible
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category=LightSense, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorLights, IgnoreLightTags", AdvancedDisplay = 5))
	static float CalculateLightLevel(UObject* WorldContextObject, const FVector& SurfacePos, const TArray<TSubclassOf<AActor>> ActorLights, TArray<FHitResult>& OutHits, TArray<FName> IgnoreLightTags, const float TraceDistance = 2000.f, const bool bUsePointLights = true, const bool bUseSpotlights = true, const bool bUseDirectionalLights = true);

	/**
	 * Calculate the light level of the surface for one light component
	 * @param Light component
	 * @param SurfacePos
	 * @return float Light level
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = LightSense, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorLights, IgnoreLightTags, ActorsToIgnore", AdvancedDisplay = 5))
	static float GetSingleLightLevel(UObject* WorldContextObject, const ULightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, TArray<FName> IgnoreLightTags, const float TraceDistance = 2000.f, const ECollisionChannel Channel = ECC_Visibility, const bool bUsePointLights = true, const bool bUseSpotlights = true, const bool bUseDirectionalLights = true, const bool bDebug = false);

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginPlay() override;

	/**
	 * Gets the current light level of the actor
	 * @return float
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentLightLevel() const;

	ULightSenseComponent();

	/** Actors to get light components from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> LightActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsePointLights = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseDirectionalLights = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseSpotlights = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> IgnoreLightComponentTags;

private:
	
	UFUNCTION()
	static float GetSpotLightLevel(const UWorld* World, const USpotLightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, const ECollisionChannel Channel = ECC_Visibility);

	UFUNCTION()
	static float GetPointLightLevel(const UWorld* World, const UPointLightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, const ECollisionChannel Channel = ECC_Visibility);

	UFUNCTION()
	static float GetDirectionalLightLevel(const UWorld* World, const UDirectionalLightComponent* Light, const FVector& SurfacePos, TArray<AActor*> ActorsToIgnore, FHitResult& OutHit, const float TraceDistance = 2000.f, const ECollisionChannel Channel = ECC_Visibility);

	/** The current light level */
	UPROPERTY()
	float LightLevel = 0.f;

	/** Distance to trace for blocking objects while calculating for directional light level */
	UPROPERTY(EditDefaultsOnly)
	float LightTraceDistance = 2000.f;
};
