// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "UW_InventoryGrid.generated.h"

DECLARE_DELEGATE_OneParam(DisplayInventoryDELEGATE, UInventorySystem* )
/**
 * 
 */
UCLASS()
class FARMINGGAME_API UUW_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UWrapBox* BOX_Grid;

	DisplayInventoryDELEGATE DisplayInventory;

	UPROPERTY()
	UInventorySystem* InventoryComponent;
	
	void NativePreConstruct() override;
};
