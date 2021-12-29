#pragma once

#include "CoreMinimal.h"
#include "Interaction/Interactable.h"
#include "Viewable.generated.h"

class UBookData;

UCLASS()
class SPOOKYGAME_API AViewable : public AInteractable
{
	GENERATED_BODY()
	
public:
	/* Item data that contains name, description, etc */
	UPROPERTY(EditAnywhere, Category = "Interaction|Data")
	UBookData* BookData = nullptr;
	
	/** Returns the item data */
	UFUNCTION(BlueprintCallable, Category = "Interaction|Data")
	UBookData* GetBookData() const;
};
