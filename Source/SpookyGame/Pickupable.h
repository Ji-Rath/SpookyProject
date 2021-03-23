#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickupable.generated.h"

class UTriggerComponent;
class UStaticMeshComponent;

UCLASS()
class SPOOKYGAME_API APickupable : public AInteractable
{
	GENERATED_BODY()
	
public:
	APickupable();

	void OnInteract_Implementation(AActor* Interactor) override;

	UPROPERTY(EditAnywhere, Category = "Pickupable")
	int Amount;

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetItemMesh() const { return ItemMesh ? ItemMesh : nullptr; };

protected:
	UPROPERTY(VisibleAnywhere)
	UTriggerComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
};