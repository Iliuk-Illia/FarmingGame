// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmingGamePlayerController.h"

#include "EnhancedInputComponent.h"
#include "InventorySystem.h"

class UEnhancedInputLocalPlayerSubsystem;

void AFarmingGamePlayerController::Inventory()
{
	
}

void AFarmingGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
/*
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InteractMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(this->InputComponent))
		{
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered,this, &AFarmingGamePlayerController::Inventory);
		}
	*/
}
