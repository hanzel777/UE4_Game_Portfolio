// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include <Blueprint\WidgetBlueprintLibrary.h>
#include <Blueprint\WidgetTree.h>
#include <Kismet\GamePlayStatics.h>

#define MaxSize 99

void UInventory::NativeConstruct()
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
			slot->SetSlotType(EItemSlotType::Inventory);
			slot->invenswapDelegate.BindUObject(this, &UInventory::SwapSlot);
			slots.Add(slot);
			slot = nullptr;
		}
	}
}

void UInventory::InitInventory(UDataTable* table)
{
	inventoryTable = table;

	//아이템 테이블의 모든 아이템 정보를 받아온다.
	TArray<FItemData*> itemDatas;
	inventoryTable->GetAllRows<FItemData>("error", itemDatas);

	//아이템 테이블의 슬롯 번호(이름)을 받아온다.
	TArray<FName> itemIndexes;
	itemIndexes = inventoryTable->GetRowNames();

	for (int32 i = 0; i < itemIndexes.Num(); i++)
	{
		if (i >= slots.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("overflow inventory.cpp Line: %d / itemIndexes.Num() == %d, Stop %d"), __LINE__, itemIndexes.Num(), i);
			break;
		}
		//아이템 데이터 테이블의 데이터를 아이템 슬롯에 넘겨준다.
		if (itemIndexes[i].ToString() != "0" && FCString::Atoi(*itemIndexes[i].ToString()) == 0) continue;
		slots[FCString::Atoi(*itemIndexes[i].ToString())]->SetItemData(*itemDatas[i]);
		//오버플로우 방지
	}
}

int32 UInventory::GainItem(FItemData itemData)
{
	UE_LOG(LogTemp, Warning, TEXT("Inven Gain: %s"), *itemData.itemName);

	//소비 아이템의 경우 이름이 같은 아이템을 가진 슬롯을 찾아 더하고 그 슬롯이 아이템 최댓값을 넘기면 남은 갯수를 더할 슬롯을 찾아서 반복
	if (itemData.itemType == EItemType::Consum)
	{
		for (int32 i = 0; i < slots.Num(); i++)
		{
			if (slots[i]->GetItemData().itemName == itemData.itemName)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *slots[i]->GetItemData().itemName);
				UE_LOG(LogTemp, Warning, TEXT("%d"), slots[i]->GetSlotNum());
				itemData.itemCount += slots[i]->GetItemData().itemCount;
				if (itemData.itemCount > MaxSize)
				{
					int32 over = itemData.itemCount - MaxSize;
					itemData.itemCount = MaxSize;
					slots[i]->SetItemData(itemData);
					inventoryTable->AddRow(*FString::FromInt(i), itemData);
					itemData.itemCount = over;
				}
				else
				{
					slots[i]->SetItemData(itemData);
					inventoryTable->AddRow(*FString::FromInt(i), itemData);
					return 0;
				}
			}
		}

	}

	//같은 소비아이템을 가지는 슬롯이 없거나 최대 갯수를 채워 새로운 슬롯에 들어가야 될때,
	//소비 아이템이 아닐 때,
	UE_LOG(LogTemp, Warning, TEXT("???"));
	for (int32 i = 0; i < slots.Num(); i++)
	{
		if (slots[i]->GetItemData().itemName == "")
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *slots[i]->GetItemData().itemName);
			UE_LOG(LogTemp, Warning, TEXT("%d"), slots[i]->GetSlotNum());
			slots[i]->SetItemData(itemData);
			inventoryTable->AddRow(*FString::FromInt(i), itemData);
			return 0;
		}
	}

	//아이템을 넣을 곳이 없다면
	return itemData.itemCount;
}

void UInventory::InsertItem(const FItemData itemData, const int32 slotNum)
{
	for (auto slot : slots)
	{
		if (slot->GetSlotNum() == slotNum)
		{
			slot->SetItemData(itemData);
			inventoryTable->AddRow(*FString::FromInt(slot->GetSlotNum()), itemData);
		}
	}
}

void UInventory::SwapSlot(const int32 from, const int32 to)
{
	//두 슬롯에 있는 아이템이 다른 아이템이거나 같더라도 아이템 타입이 소비가 아닐 시, 단순 데이터 교체
	if (slots[to]->GetItemData().itemName != slots[from]->GetItemData().itemName || slots[to]->GetItemData().itemType != EItemType::Consum)
	{
		FItemData tempData = slots[to]->GetItemData();
		slots[to]->SetItemData(slots[from]->GetItemData());
		slots[from]->SetItemData(tempData);

		inventoryTable->AddRow(*FString::FromInt(to), slots[to]->GetItemData());
		if (slots[from]->GetItemData().itemName != "")
		{
			inventoryTable->AddRow(*FString::FromInt(from), slots[from]->GetItemData());
		}
		else
		{
			inventoryTable->RemoveRow(*FString::FromInt(from));
		}
	}
	//아이템의 이름이 같고 소비 시, 
	else
	{
		switch (slots[to]->GetItemData().itemType)
		{
			//아이템 타입이 소비일 때
			case EItemType::Consum:
				//드랍된 위치의 슬롯의 아이템 갯수를 받아와 드래그된 위치의 아이템 갯수를 더한다.
				int32 cnt = slots[to]->GetItemData().itemCount;
				cnt += slots[from]->GetItemData().itemCount;

				//더해진 수가 아이템 소지의 최대 갯수보다 크다면
				if (cnt > MaxSize)
				{
					//드랍된 슬롯의 경우 아이템 데이터를 복사해 갯수를 최대 크기로 대입하고 그 데이터를 슬롯에 대입
					FItemData tempDataTo = slots[to]->GetItemData();
					tempDataTo.itemCount = MaxSize;
					slots[to]->SetItemData(tempDataTo);

					//드래그된 슬롯의 경우 아이템 데이터를 복사해 더해진 갯수에서 최대크기를 뺀 값을 대입
					FItemData tempDataFrom = slots[from]->GetItemData();
					tempDataFrom.itemCount = cnt - MaxSize;
					slots[from]->SetItemData(tempDataFrom);

					inventoryTable->AddRow(*FString::FromInt(to), slots[to]->GetItemData());
					inventoryTable->AddRow(*FString::FromInt(from), slots[from]->GetItemData());
				}
				//더해진 수가 아이템 소지 최대 갯수보다 작다면
				else
				{
					//더한 값을 대입
					FItemData tempDataTo = slots[to]->GetItemData();
					tempDataTo.itemCount = cnt;
					slots[to]->SetItemData(tempDataTo);

					//슬롯의 데이터를 초기화
					FItemData tempDataFrom = slots[from]->GetItemData();
					tempDataFrom.RemoveData();
					slots[from]->SetItemData(tempDataFrom);

					inventoryTable->AddRow(*FString::FromInt(to), slots[to]->GetItemData());
					inventoryTable->RemoveRow(*FString::FromInt(from));
				}
				break;
		}
	}
}

int32 UInventory::TotalCountOfItemName(const FString name)
{
	int32 totalCount = 0;

	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == name)
		{
			totalCount += slot->GetItemData().itemCount;
		}
	}

	return totalCount;
}

void UInventory::UsedItemFromQuick(const FString name)
{
	bool bFindFirst = false;
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == name)
		{
			slot->itemData.bReady = false;
			if (!bFindFirst)
			{
				FItemData temp = slot->GetItemData();
				temp.itemCount--;
				if (temp.itemCount <= 0)
				{
					temp.RemoveData();
					slot->SetItemData(temp);
					inventoryTable->RemoveRow(*FString::FromInt(slot->GetSlotNum()));
				}
				else
				{
					slot->SetItemData(temp);
					temp.bReady = true;
					inventoryTable->AddRow(*FString::FromInt(slot->GetSlotNum()), temp);
				}
				bFindFirst = true;
			}
		}
	}
}

const int32 UInventory::RemoveItem(const int32 slotNum)
{
	inventoryTable->RemoveRow(*FString::FromInt(slotNum));

	int32 count = 0;
	for (auto slot : slots)
	{
		if (slot->GetSlotNum() == slotNum)
		{
			FItemData temp = slot->GetItemData();
			count = temp.itemCount;
			temp.RemoveData();
			slot->SetItemData(temp);
			break;
		}
	}

	return count;
}

void UInventory::ReadyItem(const int32 slotNum)
{
	for (auto slot : slots)
	{
		if (slot->GetSlotNum() == slotNum)
		{
			slot->itemData.bReady = true;
			slot->itemData.currentTime = 0;

			inventoryTable->AddRow(*FString::FromInt(slotNum), slot->GetItemData());

			return;
		}
	}
}

TArray<UItemSlotWidget*> UInventory::GetSlots()
{
	return slots;
}

FReply UInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void UInventory::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!OutOperation)
	{
		OutOperation = NewObject<UDragDropOperation>();
		OutOperation->DefaultDragVisual = this;
	}
}