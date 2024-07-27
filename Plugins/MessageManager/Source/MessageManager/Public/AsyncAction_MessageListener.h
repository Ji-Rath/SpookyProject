// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_MessageListener.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReceiveMessage, const FInstancedStruct&, Data);

/**
 * 
 */
UCLASS()
class MESSAGEMANAGER_API UAsyncAction_MessageListener : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, meta=(WorldContext="WorldContextObject"), Category="Message Manager")
	static UAsyncAction_MessageListener* ListenForMessages(UObject* WorldContextObject, FGameplayTag Tag);

	virtual void Activate() override;

	UPROPERTY()
	UObject* WorldContext;

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY(BlueprintAssignable)
	FReceiveMessage OnReceiveMessage;

	UFUNCTION()
	void ReceiveMessage(const FInstancedStruct& MessageData);
};
