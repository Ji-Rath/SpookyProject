#include "Viewable.h"

#include "BookData.h"

UBookData* AViewable::GetBookData() const
{
	return BookData;
}

FText AViewable::GetName() const
{
	FText DisplayName = Super::GetName();
	if (BookData)
	{
		DisplayName = BookData->Name;
	}
	return DisplayName;
}
