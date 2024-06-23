// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem.h"
#include "GameFramework/SaveGame.h"
#include "SaveData_PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGGAME_API USaveData_PlayerData : public USaveGame
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FSlotStruct> InventoryContent;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> HotbarContent;
};
