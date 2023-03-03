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

	//������ ���̺��� ��� ������ ������ �޾ƿ´�.
	TArray<FItemData*> itemDatas;
	inventoryTable->GetAllRows<FItemData>("error", itemDatas);

	//������ ���̺��� ���� ��ȣ(�̸�)�� �޾ƿ´�.
	TArray<FName> itemIndexes;
	itemIndexes = inventoryTable->GetRowNames();

	for (int32 i = 0; i < itemIndexes.Num(); i++)
	{
		if (i >= slots.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("overflow inventory.cpp Line: %d / itemIndexes.Num() == %d, Stop %d"), __LINE__, itemIndexes.Num(), i);
			break;
		}
		//������ ������ ���̺��� �����͸� ������ ���Կ� �Ѱ��ش�.
		if (itemIndexes[i].ToString() != "0" && FCString::Atoi(*itemIndexes[i].ToString()) == 0) continue;
		slots[FCString::Atoi(*itemIndexes[i].ToString())]->SetItemData(*itemDatas[i]);
		//�����÷ο� ����
	}
}

int32 UInventory::GainItem(FItemData itemData)
{
	UE_LOG(LogTemp, Warning, TEXT("Inven Gain: %s"), *itemData.itemName);

	//�Һ� �������� ��� �̸��� ���� �������� ���� ������ ã�� ���ϰ� �� ������ ������ �ִ��� �ѱ�� ���� ������ ���� ������ ã�Ƽ� �ݺ�
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

	//���� �Һ�������� ������ ������ ���ų� �ִ� ������ ä�� ���ο� ���Կ� ���� �ɶ�,
	//�Һ� �������� �ƴ� ��,
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

	//�������� ���� ���� ���ٸ�
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
	//�� ���Կ� �ִ� �������� �ٸ� �������̰ų� ������ ������ Ÿ���� �Һ� �ƴ� ��, �ܼ� ������ ��ü
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
	//�������� �̸��� ���� �Һ� ��, 
	else
	{
		switch (slots[to]->GetItemData().itemType)
		{
			//������ Ÿ���� �Һ��� ��
			case EItemType::Consum:
				//����� ��ġ�� ������ ������ ������ �޾ƿ� �巡�׵� ��ġ�� ������ ������ ���Ѵ�.
				int32 cnt = slots[to]->GetItemData().itemCount;
				cnt += slots[from]->GetItemData().itemCount;

				//������ ���� ������ ������ �ִ� �������� ũ�ٸ�
				if (cnt > MaxSize)
				{
					//����� ������ ��� ������ �����͸� ������ ������ �ִ� ũ��� �����ϰ� �� �����͸� ���Կ� ����
					FItemData tempDataTo = slots[to]->GetItemData();
					tempDataTo.itemCount = MaxSize;
					slots[to]->SetItemData(tempDataTo);

					//�巡�׵� ������ ��� ������ �����͸� ������ ������ �������� �ִ�ũ�⸦ �� ���� ����
					FItemData tempDataFrom = slots[from]->GetItemData();
					tempDataFrom.itemCount = cnt - MaxSize;
					slots[from]->SetItemData(tempDataFrom);

					inventoryTable->AddRow(*FString::FromInt(to), slots[to]->GetItemData());
					inventoryTable->AddRow(*FString::FromInt(from), slots[from]->GetItemData());
				}
				//������ ���� ������ ���� �ִ� �������� �۴ٸ�
				else
				{
					//���� ���� ����
					FItemData tempDataTo = slots[to]->GetItemData();
					tempDataTo.itemCount = cnt;
					slots[to]->SetItemData(tempDataTo);

					//������ �����͸� �ʱ�ȭ
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