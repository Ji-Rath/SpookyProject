#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "BookData.generated.h"

UCLASS()
class SPOOKYGAME_API UBookData : public UItemData
{
	GENERATED_BODY()
	
public:
	/** Text to display when interacted with */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UBookData() {
		Text = FText::FromString("Text Body");
	}
};

