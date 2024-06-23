// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UW_InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class FARMINGGAME_API UUW_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UBorder* BRD_Border;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UImage* IMG_Icon;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* TXT_Quantity;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	USizeBox* BOX_Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	//UPROPERTY(Category=Test, EditDefaultsOnly)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int Quantity;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	UInventorySystem* InventoryComponent;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* DataTable;
	
	void NativePreConstruct() override;
};
