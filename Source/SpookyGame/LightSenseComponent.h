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
	UFUNCTION(BlueprintCallable)
	float GetLightLevel(TArray<ULightComponent*> Lights, const FVector& SurfacePos) const;

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetVisibility() const;

	ULightSenseComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ULightComponent*> LightArray;

private:
	float GetSpotLightLevel(USpotLightComponent* Light, const FVector& SurfacePos) const;

	float GetPointLightLevel(UPointLightComponent* Light, const FVector& SurfacePos) const;

	float GetDirectionalLightLevel(UDirectionalLightComponent* Light, const FVector& SurfacePos) const;

	/** Distance to line trace when doing light check */
	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 1000.f;

	UPROPERTY()
	float LightVisibility = 0.f;
};
