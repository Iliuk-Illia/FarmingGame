// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_InventoryGrid.h"

#include "UI/UW_InventorySlot.h"

void UUW_InventoryGrid::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (InventoryComponent != nullptr)
	{
		InventoryComponent->Content;
		for (auto Element : InventoryComponent->Content)
		{
			//CreateWidget(nullptr, UUW_InventorySlot.);
		}
	}
}
