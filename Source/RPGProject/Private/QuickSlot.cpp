// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"
#include "ItemSlotWidget.h"
#include "RPGPlayerController.h"
#include <Blueprint\WidgetTree.h>

void UQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	UItemSlotWidget* slot = nullptr;
	for (auto widget : widgets)
	{
		slot = Cast<UItemSlotWidget>(widget);
		if (slot)
		{
			slot->SetSlotType(EItemSlotType::Quick);
			slot->quickSwapDelegate.BindUObject(this, &UQuickSlot::SwapData);
			slots.Add(slot);
			slot = nullptr;
		}
	}

	//플레이어 폰을 받고 컨트롤러를 받아와 RPG플레이어로 형변환
	//성공하면 아이템 데이터 배열을 아이템 슬롯 갯수만큼 초기화
	ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetOwningPlayerPawn()->GetController());
	if (rpgController)
	{
		rpgController->itemDatas.Init(nullptr, slots.Num());
	}

	//스킬 슬롯 배열 인자의 스킬 슬롯 번호에 맞는 스킬 데이터 배열 인덱스의 스킬 데이터를 스킬 슬롯의 스킬 데이터의 주소 값으로 초기화
	for (auto itemSlot : slots)
	{
		rpgController->itemDatas[itemSlot->GetSlotNum()] = &itemSlot->itemData;
	}
}

void UQuickSlot::InitQuickSlot(UDataTable* table)
{
	quickSlotTable = table;

	//테이블의 모든 아이템 정보를 받아온다.
	TArray<FItemData*> itemDatas;
	quickSlotTable->GetAllRows<FItemData>("error", itemDatas);

	//테이블의 슬롯 번호(이름)을 받아온다.
	TArray<FName> itemIndexes;
	itemIndexes = quickSlotTable->GetRowNames();

	for (uint8 i = 0; i < itemIndexes.Num(); i++)
	{
		//아이템 데이터 테이블의 데이터를 아이템 슬롯에 넘겨준다.
		if (itemIndexes[i].ToString() != "0" && FCString::Atoi(*itemIndexes[i].ToString()) == 0) continue;
		slots[FCString::Atoi(*itemIndexes[i].ToString())]->SetItemData(*itemDatas[i]);
		//오버플로우 방지
		if (i >= slots.Num())break;
	}
}

void UQuickSlot::SetQuickSlot(const FItemData itemData, const int32 slotNum, const int32 totalCount)
{
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == itemData.itemName)
		{
			SwapData(slot->GetSlotNum(), slotNum);
			return;
		}
	}

	FItemData temp = itemData;
	temp.itemCount = totalCount;
	slots[slotNum]->SetItemData(temp);
	quickSlotTable->AddRow(*FString::FromInt(slotNum), slots[slotNum]->GetItemData());
}

void UQuickSlot::SwapData(const int32 from, const int32 to)
{
	if (from == to) return;

	FItemData tempData = slots[to]->GetItemData();
	slots[to]->SetItemData(slots[from]->GetItemData());
	slots[from]->SetItemData(tempData);

	quickSlotTable->AddRow(*FString::FromInt(to), slots[to]->GetItemData());
	if (slots[from]->GetItemData().itemName != "")
	{
		quickSlotTable->AddRow(*FString::FromInt(from), slots[from]->GetItemData());
	}
	else
	{
		quickSlotTable->RemoveRow(*FString::FromInt(from));
	}
}

void UQuickSlot::UsedItemFromInven(const FString name)
{
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == name)
		{
			FItemData temp = slot->GetItemData();
			if (temp.itemCount <= 0) return;

			temp.itemCount--;
			temp.bReady = false;
			slot->SetItemData(temp);
			quickSlotTable->AddRow(*FString::FromInt(slot->GetSlotNum()), slot->GetItemData());
			return;
		}
	}
}

void UQuickSlot::GainItem(const FItemData data)
{
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == data.itemName)
		{
			FItemData temp = slot->GetItemData();
			temp.itemCount += data.itemCount;
			slot->SetItemData(temp);
			quickSlotTable->AddRow(*FString::FromInt(slot->GetSlotNum()), slot->GetItemData());
			return;
		}
	}
}

void UQuickSlot::RemoveItem(const int32 slotNum)
{
	FItemData temp = slots[slotNum]->GetItemData();
	temp.RemoveData();
	slots[slotNum]->SetItemData(temp);
	quickSlotTable->RemoveRow(*FString::FromInt(slotNum));
}

void UQuickSlot::SubtractItem(const FString itemName, const int32 count)
{
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == itemName)
		{
			slot->itemData.itemCount -= count;
			quickSlotTable->AddRow(*FString::FromInt(slot->GetSlotNum()), slot->GetItemData());
			return;
		}
	}
}

TArray<UItemSlotWidget*> UQuickSlot::GetSlots()
{
	return slots;
}