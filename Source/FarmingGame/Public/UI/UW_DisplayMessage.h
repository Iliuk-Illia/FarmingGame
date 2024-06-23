// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UW_DisplayMessage.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowMessage, FText, Message);

/**
 * 
 */
UCLASS()
class FARMINGGAME_API UUW_DisplayMessage : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UBorder* Box_Message;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* TXT_Message;
public:
	
	UPROPERTY(BlueprintReadWrite)
	FText Message;
	
	//UPROPERTY(BlueprintAssignable)
	//FShowMessage ShowMessageDelegate;
	
	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void ShowMessage(FText message1);
};
