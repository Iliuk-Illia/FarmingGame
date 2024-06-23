// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FarmingGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGGAME_API AFarmingGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	/*
	UPROPERTY(BlueprintReadWrite)
	FString SlotName;

	UPROPERTY(BlueprintReadWrite)
	USaveData_Level* SaveData_Level;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddedActors();
	*/
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
