// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "RPGPlayer.h"
#include "RPGPlayerController.h"
#include "ItemData.h"
#include "Blueprint/DragDropOperation.h"
#include <Blueprint\WidgetTree.h>
#include <Blueprint\WidgetLayoutLibrary.h>

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	UInventory* inven = nullptr;
	USkillListView* skillList = nullptr;
	USkillSlot* skillSl = nullptr;
	UQuickSlot* quick = nullptr;
	UEquipment* equip = nullptr;
	for (auto widget : widgets)
	{
		inven = Cast<UInventory>(widget);
		if (inven)
		{
			inven->SetVisibility(ESlateVisibility::Hidden);
			inventory = inven;
			inven = nullptr;
			continue;
		}

		skillList = Cast<USkillListView>(widget);
		if (skillList)
		{
			skillList->SetVisibility(ESlateVisibility::Hidden);
			skillListView = skillList;
			skillList = nullptr;
			continue;
		}
		
		equip = Cast<UEquipment>(widget);
		if (equip)
		{
			equip->SetVisibility(ESlateVisibility::Hidden);
			equipment = equip;
			equip = nullptr;
			continue;
		}

		quick = Cast<UQuickSlot>(widget);
		if (quick)
		{
			quickSlot = quick;
			quick = nullptr;
			continue;
		}

		skillSl = Cast<USkillSlot>(widget);
		if (skillSl)
		{
			skillSlot = skillSl;
			skillSl = nullptr;
			continue;
		}
	}

	if (quickSlot)
	{
		for (auto& slot : quickSlot->GetSlots())
		{
			slot->invenToQuick.BindUObject(this, &UMainUI::SetQuickSlot);
		}
	}

	ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetOwningPlayerPawn()->GetController());
	if (rpgController)
	{
		rpgController->useItemDelegate.BindUObject(this, &UMainUI::UsedItem);
	}
}

UInventory* UMainUI::GetInventory() const
{
	return inventory;
}

USkillListView* UMainUI::GetSkillListView() const
{
	return skillListView;
}

USkillSlot* UMainUI::GetSkillSlot() const
{
	return skillSlot;
}

UQuickSlot* UMainUI::GetQuickSlot() const
{
	return quickSlot;
}

UEquipment* UMainUI::GetEquipment() const
{
	return equipment;
}

//PRG�÷��̾� ��Ʈ�ѷ� ��������Ʈ ���ε� �� �������� useitem�Լ��� �۵��ϸ� �÷��̾�� main���� �ٽ� ���ƿ�
void UMainUI::UsedItem(const FString name)
{
	UE_LOG(LogTemp, Warning, TEXT("UMainUI::UsedItem"));
	quickSlot->UsedItemFromInven(name);
	inventory->UsedItemFromQuick(name);
	ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
	if (player)
	{
		ItemData::UsedItem(player, name);
	}
}

void UMainUI::GainItem(const FItemData data)
{
	//�κ��丮�� ���� ���ϰ� ���� ���� ��ȯ
	int32 remainCnt = inventory->GainItem(data);
	UE_LOG(LogTemp, Warning, TEXT("%d"), remainCnt);

	//�κ��丮�� ���� ���� ������ �ִٸ� ���� �����Ϳ��� ������ŭ ���� ���Ѵ�.
	if (remainCnt > 0)
	{
		FItemData temp = data;
		temp.itemCount -= remainCnt;
		quickSlot->GainItem(temp);
	}
	//�κ��丮�� ��� �� ���� ������ ���ٸ� ���� ������ �����͸� �״�� ����
	else
	{
		quickSlot->GainItem(data);
	}
}

void UMainUI::SetQuickSlot(const FItemData itemData, const int32 slotNum)
{
	int32 totalCount = 0;
	for (auto& slot : inventory->GetSlots())
	{
		if (itemData.itemName == slot->GetItemData().itemName)
		{
			totalCount += slot->GetItemData().itemCount;
		}
	}

	quickSlot->SetQuickSlot(itemData, slotNum, totalCount);
}

bool UMainUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("drop"));
	if (InOperation)
	{
		UItemSlotWidget* visual = Cast<UItemSlotWidget>(InOperation->DefaultDragVisual);
		if (visual)
		{
			if (visual->GetSlotType() == EItemSlotType::Inventory)
			{
				int32 count = inventory->RemoveItem(visual->GetSlotNum());

				quickSlot->SubtractItem(visual->GetItemData().itemName, count);
			}
			else if (visual->GetSlotType() == EItemSlotType::Quick)
			{
				quickSlot->RemoveItem(visual->GetSlotNum());
			}
			else
			{
				equipment->RemoveItem(visual->GetSlotNum());
			}
		}

		UInventory* inven = Cast<UInventory>(InOperation->DefaultDragVisual);
		if (inven)
		{
			FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayerPawn()->GetController());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *mousePos.ToString());
			inventory->SetPositionInViewport(mousePos, false);
		}

		return true;
	}
	return false;
}

void UMainUI::InvenCoolTime(float InDeltaTime)
{
	for (auto slot : inventory->GetSlots())
	{
		if (!slot->GetItemData().bReady)
		{
			slot->itemData.currentTime += InDeltaTime;
			if (slot->GetItemData().currentTime >= slot->GetItemData().delayTime)
			{
				inventory->ReadyItem(slot->GetSlotNum());
			}
		}
	}
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	InvenCoolTime(InDeltaTime);
}