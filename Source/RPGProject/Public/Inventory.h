// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UInventory : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TArray<UItemSlotWidget*> slots;
	UDataTable* inventoryTable;

protected:
	virtual void NativeConstruct() override;

public:
	void InitInventory(UDataTable* table);

	UFUNCTION(BlueprintCallable)
	int32 GainItem(FItemData itemData);

	void InsertItem(const FItemData itemData, const int32 slotNum);

	UFUNCTION()
	void SwapSlot(const int32 from, const int32 to);

	void UsedItemFromQuick(const FString name);

	int32 TotalCountOfItemName(const FString name);

	TArray<UItemSlotWidget*> GetSlots();

	const int32 RemoveItem(const int32 slotNum);

	void ReadyItem(const int32 slotNum);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};