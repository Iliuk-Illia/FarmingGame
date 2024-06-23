// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData_PlayerData.h"
#include "Engine/GameInstance.h"
#include "FarmingGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBushInteract, AActor*, TargetActor);

/**
 * 
 */
UCLASS()
class FARMINGGAME_API UFarmingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;
public:

	UPROPERTY(BlueprintReadWrite)
	FString SlotName = "FarmingGameData";

	UPROPERTY(BlueprintReadWrite)
	USaveData_PlayerData* SaveData_PlayerData;
	
	UPROPERTY(BlueprintReadWrite)
	int SlotIndex=0;

	UPROPERTY(BlueprintAssignable)
	FOnBushInteract OnBushInteract;
};
