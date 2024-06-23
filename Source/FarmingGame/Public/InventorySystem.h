// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "UI/UW_DisplayMessage.h"

#include "InventorySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLocalInteract, AActor*, TargetActor, AActor*, Interactor);


USTRUCT(BlueprintType, Blueprintable)
struct FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Quantity;
	
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemStruct: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeToHarvest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeToC_Stage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeToB_Stage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeToA_Stage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_Starter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_C;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_C_Harvest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_B;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_B_Harvest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SM_A_Harvest;
};

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FARMINGGAME_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystem();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDataComponent> ItemDataComponentClass;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* DataTable;

	UPROPERTY()
	AFarmingGameCharacter* Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* InteractMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;
	
	UPROPERTY(EditDefaultsOnly)
	int InventorySize = 15;
	
	UPROPERTY(EditDefaultsOnly)
	int InteractoinRange = 150;

	UPROPERTY()
	AActor* LookAtActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UUW_DisplayMessage* MessageWidget;

	UFUNCTION(BlueprintCallable)
	void DebugFunction();
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<FSlotStruct> Content;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnLocalInteract OnLocalInteracted;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Interact();

	UFUNCTION(BlueprintCallable)
	void ConsumeItem(int Index);

	TTuple<bool, int> QueryInventory(FName ItemID, int Quantity);
	
	UFUNCTION(BlueprintCallable)
	int AddToInventory(FName ItemId, int Quantity);
	
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int Index, bool RemoveWholeStack, bool IsConsuned);

	UFUNCTION(BlueprintCallable)
	void DropItem(FName ItemId, int Quantity);

	FItemStruct* GetItemData(FName ItemId);

	FVector GetDropLocation();
	
	UFUNCTION(BlueprintCallable)
	void TransferSlots(int SourceIndex, UInventorySystem* SourceInventory, int DestinationIndex);

	UFUNCTION(BlueprintCallable)
	virtual void UpdateInventory();
	
	UFUNCTION(BlueprintCallable)
	virtual void LocalInteract(AActor* TargetActor, AActor* Interactor);

	UFUNCTION(BlueprintCallable)
	void SaveInventory();
	UFUNCTION(BlueprintCallable)
	void LoadInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void SaveInventoryNotCharacter();
	UFUNCTION(BlueprintImplementableEvent)
	void LoadInventoryNotCharacter();
	
private:
	
	UFUNCTION()
	void InteractionTrace();
	
	UFUNCTION()
	int FindSlot(FName ItemId);

	UFUNCTION()
	int GetMaxStackSize(FName ItemId);

	UFUNCTION()
	bool CreateNewStack(FName ItemId, int Quantity);
	
	UFUNCTION()
	int AnyEmptySlotsAvailable();
	
	UFUNCTION()
	void AddToStack(int index, int Quantity);
	
};