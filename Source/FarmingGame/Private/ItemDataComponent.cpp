// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataComponent.h"
#include "FarmingGameCharacter.h"
#include "FarmingGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UItemDataComponent::InteractWith_Implementation(AFarmingGameCharacter* Character)
{
	if (IsFullyGrown)
	{
		if (Character->Inventory->AddToInventory(ItemId.RowName, Quantity) >=0)
		{
			if (GetOwner() != nullptr)
			{
				if (!IsBush)
				{
					GetOwner()->Destroy();
					
					UKismetSystemLibrary::PrintString(GetWorld(),FString(TEXT("Destroy")));
				}
				else
				{
					Cast<UFarmingGameInstance>(GetWorld()->GetGameInstance())->OnBushInteract.Broadcast(GetOwner());
					
					UKismetSystemLibrary::PrintString(GetWorld(),FString(TEXT("Broadcst")));
				}
			}
		}
	}
}

