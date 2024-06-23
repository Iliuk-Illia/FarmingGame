// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem.h"

#include "CppPlant.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IInteractInterface.h"
#include "Kismet/GameplayStatics.h"
#include "FarmingGameCharacter.h"
#include "FarmingGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UInventorySystem::UInventorySystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInventorySystem::InteractionTrace()
{
	APlayerCameraManager* Player;
	Player = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector3d Start = Player->K2_GetActorLocation();
	FVector3d End = Player->K2_GetActorLocation() + (Player->GetActorForwardVector() * InteractoinRange);
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
	ETraceTypeQuery MyTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

	if(UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, MyTraceType ,false, actorsToIgnore, EDrawDebugTrace::None, HitResult,true))
	{
		if (LookAtActor != HitResult.GetActor() )
		{
			LookAtActor = HitResult.GetActor();

			if (LookAtActor != nullptr)
			{
				if (UKismetSystemLibrary::DoesImplementInterface(LookAtActor,UIInteractInterface::StaticClass()))
				{
					FText message = IIInteractInterface::Execute_LookAt(LookAtActor);
				
					MessageWidget->ShowMessage(message);
				}
				//UKismetSystemLibrary::PrintText(GetWorld(), message);
			}
		}
	}
	else
	{
		LookAtActor = nullptr;
		MessageWidget->ShowMessage(FText());
	}
	
}

void UInventorySystem::DebugFunction()
{
	FString str = "Element {i} : name {name}, quanity {quanity}";
	
	FFormatNamedArguments Args;

		int i = 0;
		for (auto Element : Content)
		{
			//str.Append(TEXT("Element %i : name %s, quanity %i"), i, Element.ItemID.ToString(), Element.Quantity));
			//FString::Printf(TEXT("Element %i : name, quanity %i"), i, Element.Quantity);
			
			Args.Add(TEXT("i"),i);
			Args.Add(TEXT("quanity"),Element.Quantity);

			UKismetSystemLibrary::PrintText(GetWorld(),FText::Format(FTextFormat::FromString(str), Args));
			i++;
		}
	
}

int UInventorySystem::AddToInventory(FName ItemId, int Quantity)
{
	int RemainingQuantity = Quantity;
	bool HasFailed = false;
	bool next = true;
	int index;
	int indexEmptySlot;

	while (next)
	{
		if (RemainingQuantity>0 && !HasFailed)
		{
			next = false;
		}

		index = FindSlot(ItemId);
		if (index>=0) 
		{
			AddToStack(index,1);
			--RemainingQuantity;
		}
		else
		{
			indexEmptySlot = AnyEmptySlotsAvailable();
			if (indexEmptySlot>=0) 
			{
				if (CreateNewStack(ItemId,1))
				{
					--RemainingQuantity;
				}
				else
				{
					HasFailed = true;
				}
			}
		}
	}
	
	OnInventoryUpdated.Broadcast();
	return RemainingQuantity;
}

void UInventorySystem::AddToStack(int index, int Quantity)
{
	
	FSlotStruct NewItem;
	NewItem.ItemID = Content[index].ItemID;
	NewItem.Quantity = Content[index].Quantity + Quantity;

	Content[index]=NewItem;
}

int UInventorySystem::FindSlot(FName ItemId)
{
	int i = 0;
	for (auto Element : Content)
	{
		if (Element.ItemID == ItemId)
		{
			if (Element.Quantity < GetMaxStackSize(ItemId))
			{
				return i;
			}
		}
		++i;
	}
	return -1;
}

int UInventorySystem::GetMaxStackSize(FName ItemId)
{
	FItemStruct* OutRow = DataTable->FindRow<FItemStruct>(ItemId, "");
	
	if (OutRow != nullptr)
	{
		return OutRow->StackSize;
	}

	return -1;
}

bool UInventorySystem::CreateNewStack(FName ItemId, int Quantity)
{
	int index = AnyEmptySlotsAvailable();

	if (index >=0)
	{
		FSlotStruct NewItem;
		NewItem.ItemID = ItemId;
		NewItem.Quantity = Quantity;

		Content[index]=NewItem;
		return true;
	}
	return false;
}

int UInventorySystem::AnyEmptySlotsAvailable()
{
	int i = 0;
	for (auto Element : Content)
	{
		if (Element.Quantity == 0)
		{
			return i;
		}
		++i;
	}
	return -1;
}

void UInventorySystem::RemoveFromInventory(int Index, bool RemoveWholeStack, bool IsConsuned)
{
	FName localItem = Content[Index].ItemID;
	int localQuantity = Content[Index].Quantity;

	if (RemoveWholeStack || localQuantity == 1)
	{
		Content[Index].ItemID = "";
		Content[Index].Quantity = 0;
		if (IsConsuned)
		{
			
		}
		else
		{
			DropItem(localItem, localQuantity);
		}
	}
	else
	{
		Content[Index].Quantity = Content[Index].Quantity - 1;
		if (IsConsuned)
		{
			
		}
		else
		{
			DropItem(localItem, 1);
		}
	}

	UpdateInventory();
}

void UInventorySystem::DropItem(FName ItemId, int Quantity)
{
	FItemStruct* item = GetItemData(ItemId);
	FVector location = GetDropLocation();
	ACppPlant* Actor;
	for (int i = 0; i < Quantity; ++i)
	{
		Actor = Cast<ACppPlant>(GetWorld()->SpawnActor(item->ItemClass, &location));
		Actor->FromInventory=true;
		//location = GetDropLocation();
	}
}

FItemStruct* UInventorySystem::GetItemData(FName ItemId)
{
	return DataTable->FindRow<FItemStruct>(ItemId,"");
}

FVector UInventorySystem::GetDropLocation()
{
	AActor* Player = GetOwner();
	FVector3d Start = Player->K2_GetActorLocation() + (UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Player->GetActorForwardVector(),30) * 150);
	FVector3d End = FVector3d(Start.X,Start.Y,Start.Z-500);
	TArray<AActor*> actorsToIgnore;
	FHitResult HitResult;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
	
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceType, false, actorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	
	return HitResult.Location;
}

void UInventorySystem::TransferSlots(int SourceIndex, UInventorySystem* SourceInventory, int DestinationIndex)
{
	if (SourceInventory != nullptr)
	{
		FSlotStruct localSlotContent = SourceInventory->Content[SourceIndex];

		if (DestinationIndex < 0)
		{
			SourceInventory->Content[SourceIndex] = Content[SourceIndex];
			//Content[DestinationIndex] = localSlotContent;
		}
		else
		{
			if (localSlotContent.ItemID == Content[DestinationIndex].ItemID)
			{
				//FSlotStruct slotContent = FSlotStruct();
				
				int ValueForClamp = (localSlotContent.Quantity + Content[DestinationIndex].Quantity) - GetMaxStackSize(localSlotContent.ItemID);
				int clampValue = UKismetMathLibrary::Clamp(ValueForClamp, 0, GetMaxStackSize(localSlotContent.ItemID));
				if (clampValue > 0)
				{
					SourceInventory->Content[SourceIndex].ItemID = localSlotContent.ItemID;
					SourceInventory->Content[SourceIndex].Quantity = clampValue;
				}
				else
				{
					SourceInventory->Content[SourceIndex].ItemID = "";
				}
				ValueForClamp = (localSlotContent.Quantity + Content[DestinationIndex].Quantity);
				clampValue = UKismetMathLibrary::Clamp(ValueForClamp, 0, GetMaxStackSize(localSlotContent.ItemID));
				
				Content[DestinationIndex].ItemID = localSlotContent.ItemID;
				Content[DestinationIndex].Quantity = clampValue;
				
				OnInventoryUpdated.Broadcast();
				SourceInventory->OnInventoryUpdated.Broadcast();
			}
			else
			{
			
				SourceInventory->Content[SourceIndex] = Content[DestinationIndex];
				Content[DestinationIndex] = localSlotContent;
			}
		}
		OnInventoryUpdated.Broadcast();
		SourceInventory->OnInventoryUpdated.Broadcast();
	}
}

void UInventorySystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character != nullptr)
	{
		InteractionTrace();
	}
}

void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();
	LoadInventory();

	Character = Cast<AFarmingGameCharacter>(GetOwner());
	if (Character != nullptr)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				
				Subsystem->AddMappingContext(InteractMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered,this, &UInventorySystem::Interact);

			}
		}
	}
	
	if (Content.IsEmpty())
	{
		FSlotStruct init;
		init.Quantity = 0;
		init.ItemID = FName();
		Content.Init(init,InventorySize);
	}
	
	//LoadInventory();

	OnInventoryUpdated.AddDynamic(this, &UInventorySystem::SaveInventory);
}

void UInventorySystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(InteractMappingContext);
		}
	}
}

void UInventorySystem::Interact()
{
	if (LookAtActor != nullptr)
	{
		if (UKismetSystemLibrary::DoesImplementInterface(LookAtActor,UIInteractInterface::StaticClass()))
		{
			UActorComponent* ActorComp = LookAtActor->GetComponentByClass(ItemDataComponentClass);
			if (ActorComp != nullptr)
			{
				IIInteractInterface::Execute_InteractWith(ActorComp, Character);
			}
			else
			{
				LookAtActor->SetOwner(Cast<AFarmingGameCharacter>(GetOwner())->GetController());

				LocalInteract(LookAtActor,GetOwner());
			}
		}
	}
}

void UInventorySystem::ConsumeItem(int Index)
{
	FName localItemId = Content[Index].ItemID;
	int localQuantity = Content[Index].Quantity;

	RemoveFromInventory(Index,false,true);
	UpdateInventory();
}

TTuple<bool, int> UInventorySystem::QueryInventory(FName ItemID, int Quantity)
{
	int runningTotal = 0;
	for (auto Element : Content)
	{
		if (ItemID == Element.ItemID)
		{
			runningTotal+=Element.Quantity;
		}
	}
	
	return MakeTuple(runningTotal >= Quantity,runningTotal);
}

void UInventorySystem::UpdateInventory()
{
	OnInventoryUpdated.Broadcast();
}

void UInventorySystem::LocalInteract(AActor* TargetActor, AActor* Interactor)
{
	if (TargetActor != nullptr && Interactor != nullptr)
	{
		OnLocalInteracted.Broadcast(TargetActor,Interactor);
	
		IIInteractInterface::Execute_InteractWith(TargetActor, Cast<AFarmingGameCharacter>(Interactor));
	}
}

void UInventorySystem::SaveInventory()
{
	if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		UFarmingGameInstance* GameInstance = Cast<UFarmingGameInstance>(GetWorld()->GetGameInstance());
		GameInstance->SaveData_PlayerData->InventoryContent = Content;

		UGameplayStatics::SaveGameToSlot(GameInstance->SaveData_PlayerData, GameInstance->SlotName, 0);
	}
	else
	{
		SaveInventoryNotCharacter();
	}
	
}

void UInventorySystem::LoadInventory()
{
	if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		UFarmingGameInstance* GameInstance = Cast<UFarmingGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance != nullptr)
		{
			Content = GameInstance->SaveData_PlayerData->InventoryContent;
		}
	}
	else
	{
		LoadInventoryNotCharacter();
	}
}
