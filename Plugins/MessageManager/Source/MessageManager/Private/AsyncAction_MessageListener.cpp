// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction_MessageListener.h"

#include "MessageSubsystem.h"

UAsyncAction_MessageListener* UAsyncAction_MessageListener::ListenForMessages(UObject* WorldContextObject, FGameplayTag Tag)
{
	auto Object = NewObject<UAsyncAction_MessageListener>();
	Object->RegisterWithGameInstance(WorldContextObject);

	Object->WorldContext = WorldContextObject;
	Object->Tag = Tag;
	return Object;
}

void UAsyncAction_MessageListener::Activate()
{
	auto World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		SetReadyToDestroy();
		return;
	}
	auto GI = World->GetGameInstance();
	if (!GI)
	{
		SetReadyToDestroy();
		return;
	}
	auto MessageSubsystem = GI->GetSubsystem<UMessageSubsystem>();
	if (!MessageSubsystem)
	{
		SetReadyToDestroy();
		return;
	}

	FReceiveNewMessage ReceiveNewMessage;
	ReceiveNewMessage.BindDynamic(this, &UAsyncAction_MessageListener::ReceiveMessage);
	MessageSubsystem->ListenForMessages(Tag, ReceiveNewMessage);
}

void UAsyncAction_MessageListener::ReceiveMessage(const FInstancedStruct& MessageData)
{
	OnReceiveMessage.Broadcast(MessageData);
}
