// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmingGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UFarmingGameInstance::Init()
{
	Super::Init();

	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		SaveData_PlayerData = Cast<USaveData_PlayerData>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		UGameplayStatics::SaveGameToSlot(SaveData_PlayerData,SlotName,0);
	}
	else
	{
		SaveData_PlayerData = Cast<USaveData_PlayerData>(UGameplayStatics::CreateSaveGameObject(USaveData_PlayerData::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveData_PlayerData,SlotName,0);
	}
	
}
