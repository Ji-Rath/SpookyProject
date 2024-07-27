// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MessageSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FReceiveNewMessage, const FInstancedStruct&, Data);

/**
 * Manages messages that have been broadcasted and waiting to be received by other systems
 */
UCLASS()
class MESSAGEMANAGER_API UMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	TMap<FGameplayTag, TArray<FInstancedStruct>> Messages;
	
	TMap<FGameplayTag, TArray<FReceiveNewMessage>> Listeners;

public:
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Tag"), Category="Message Manager")
	void SendMessage(const FGameplayTag& Tag, const FInstancedStruct& Data);

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm="Tag"), Category="Message Manager")
	void ListenForMessages(const FGameplayTag& Tag, const FReceiveNewMessage& Callback);

	void BroadcastMessages(const FGameplayTag& Tag);
};
