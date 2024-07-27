#pragma once

#include "CoreMinimal.h"
#include "Interaction/ItemData.h"
#include "BookData.generated.h"

UCLASS()
class SPOOKYGAME_API UBookData : public UItemInformation
{
	GENERATED_BODY()
	
public:
	/** Text to display when interacted with */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Text;
};
