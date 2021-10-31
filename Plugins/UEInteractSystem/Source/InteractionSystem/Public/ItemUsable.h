#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inventory/Pickupable.h"
#include "ItemUsable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONSYSTEM_API IItemUsable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnItemUse(AActor* User, UItemData* Item);
};