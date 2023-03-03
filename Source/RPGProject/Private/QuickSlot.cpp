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

	//�÷��̾� ���� �ް� ��Ʈ�ѷ��� �޾ƿ� RPG�÷��̾�� ����ȯ
	//�����ϸ� ������ ������ �迭�� ������ ���� ������ŭ �ʱ�ȭ
	ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetOwningPlayerPawn()->GetController());
	if (rpgController)
	{
		rpgController->itemDatas.Init(nullptr, slots.Num());
	}

	//��ų ���� �迭 ������ ��ų ���� ��ȣ�� �´� ��ų ������ �迭 �ε����� ��ų �����͸� ��ų ������ ��ų �������� �ּ� ������ �ʱ�ȭ
	for (auto itemSlot : slots)
	{
		rpgController->itemDatas[itemSlot->GetSlotNum()] = &itemSlot->itemData;
	}
}

void UQuickSlot::InitQuickSlot(UDataTable* table)
{
	quickSlotTable = table;

	//���̺��� ��� ������ ������ �޾ƿ´�.
	TArray<FItemData*> itemDatas;
	quickSlotTable->GetAllRows<FItemData>("error", itemDatas);

	//���̺��� ���� ��ȣ(�̸�)�� �޾ƿ´�.
	TArray<FName> itemIndexes;
	itemIndexes = quickSlotTable->GetRowNames();

	for (uint8 i = 0; i < itemIndexes.Num(); i++)
	{
		//������ ������ ���̺��� �����͸� ������ ���Կ� �Ѱ��ش�.
		if (itemIndexes[i].ToString() != "0" && FCString::Atoi(*itemIndexes[i].ToString()) == 0) continue;
		slots[FCString::Atoi(*itemIndexes[i].ToString())]->SetItemData(*itemDatas[i]);
		//�����÷ο� ����
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