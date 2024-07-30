// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemGroup.generated.h"

class UItemInformation;

USTRUCT(BlueprintType)
struct FItemGroup
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemGroup")
	TArray<UItemInformation*> Items;
	
	friend uint32 GetTypeHash(const FItemGroup& ItemGroup)
	{
		return GetTypeHash(ItemGroup.Items);
	}
};