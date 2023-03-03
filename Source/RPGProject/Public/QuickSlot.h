// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "ItemSlotWidget.h"
#include "QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UQuickSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	TArray<class UItemSlotWidget*> slots;

	UFUNCTION()
	void SwapData(const int32 from, const int32 to);

	UDataTable* quickSlotTable;

protected:
	virtual void NativeConstruct() override;

public:
	void InitQuickSlot(UDataTable* table);

	void UsedItemFromInven(const FString name);

	TArray<UItemSlotWidget*> GetSlots();

	void SetQuickSlot(const FItemData itemData, const int32 slotNum, const int32 totalCount);

	void GainItem(const FItemData data);

	void RemoveItem(const int32 slotNum);

	void SubtractItem(const FString itemName, const int32 count);
};
