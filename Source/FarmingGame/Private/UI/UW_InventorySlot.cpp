// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UW_InventorySlot.h"

void UUW_InventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DataTable != nullptr)
	{
		FItemStruct* OutRow1 = DataTable->FindRow<FItemStruct>(ItemID,"");
		
		if (!ItemID.IsNone())
		{
			IMG_Icon->SetBrushFromTexture(OutRow1->Thumbnail);
			TXT_Quantity->SetText(FText::FromString(FString::FromInt(Quantity)));

			IMG_Icon->SetVisibility(ESlateVisibility::Visible);
			BOX_Quantity->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			IMG_Icon->SetVisibility(ESlateVisibility::Hidden);
			BOX_Quantity->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
	else
	{
		IMG_Icon->SetVisibility(ESlateVisibility::Hidden);
		BOX_Quantity->SetVisibility(ESlateVisibility::Hidden);
	}
	
}
