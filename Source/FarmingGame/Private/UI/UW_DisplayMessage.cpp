// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_DisplayMessage.h"

void UUW_DisplayMessage::NativePreConstruct()
{
	
	TXT_Message->SetText(Message);
	if (Message.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUW_DisplayMessage::ShowMessage(FText message1)
{
	Message = message1;
	NativePreConstruct();
}
