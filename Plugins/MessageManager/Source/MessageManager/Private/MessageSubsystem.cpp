// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageSubsystem.h"

void UMessageSubsystem::SendMessage(const FGameplayTag& Tag, const FInstancedStruct& Data)
{
	auto& MessageArray = Messages.FindOrAdd(Tag);
	MessageArray.Add(Data);
	BroadcastMessages(Tag);
}

void UMessageSubsystem::ListenForMessages(const FGameplayTag& Tag, const FReceiveNewMessage& Callback)
{
	auto& ListenerArray = Listeners.FindOrAdd(Tag);
	ListenerArray.Add(Callback);
	BroadcastMessages(Tag);
}

void UMessageSubsystem::BroadcastMessages(const FGameplayTag& Tag)
{
	auto MessageArray = Messages.Find(Tag);
	if (!MessageArray) { return; } // No messages to broadcast
	
	auto ListenerArray = Listeners.Find(Tag);
	if (!ListenerArray) { return; } // No listeners to broadcast to

	bool bBound = false;
	for (const auto& Listener : *ListenerArray)
	{
		for (const auto& MessageData : *MessageArray)
		{
			if (Listener.ExecuteIfBound(MessageData))
			{
				bBound = true;
			}
		}
	}

	// Clear messages if we bound to a listener
	if (bBound)
	{
		Messages.Remove(Tag);
	}
}
