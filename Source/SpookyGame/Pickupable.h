#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "Interaction.h"
#include "Pickupable.generated.h"

class UTriggerComponent;
class UStaticMeshComponent;

UCLASS()
class SPOOKYGAME_API APickupable : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	APickupable();

	void OnInteract(const AActor* Interactor);
	void OnInteract_Implementation(const AActor* Interactor) override;

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