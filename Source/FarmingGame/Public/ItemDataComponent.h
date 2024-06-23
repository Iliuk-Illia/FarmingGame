// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractInterface.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"

#include "ItemDataComponent.generated.h"

class AFarmingGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMINGGAME_API UItemDataComponent : public UActorComponent, public IIInteractInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FDataTableRowHandle ItemId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Quantity = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsBush = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsFullyGrown = false;
	
	virtual void InteractWith_Implementation(AFarmingGameCharacter* Character) override;
};
