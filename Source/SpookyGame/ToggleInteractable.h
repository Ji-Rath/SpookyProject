#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "ToggleInteractable.generated.h"

UCLASS()
class SPOOKYGAME_API AToggleInteractable : public AInteractable
{
	GENERATED_BODY()
	
public:
	/**
	 * Toggle the status of the interactable
	 * @warning Will not toggle more than once if bOneTimeInteraction is true
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ToggleInteract(AActor* Interactor);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetState(AActor* Interactor, bool bOn);

	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetState() const;

	/** The current state of the interactable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bIsOn = false;
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void ConstructInteractable();
};

