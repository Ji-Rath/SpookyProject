#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsGrabComponent.generated.h"

class UPhysicsHandleComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPOOKYGAME_API UPhysicsGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPhysicsGrabComponent();

	void BeginPlay() override;

	/** Called to grab the in-range physics object */
	void Grab();

	UPROPERTY(EditAnywhere, Category = "Grab")
	float InteractDistance = 100.f;
private:
	UPROPERTY()
	UPhysicsHandleComponent* HandleRef;

	UPROPERTY()
	APawn* Player;
};


