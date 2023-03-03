// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "ItemDragDropOperation.h"
#include "RPGPlayer.h"
#include "RPGPlayerController.h"
#include <Blueprint\WidgetBlueprintLibrary.h>

EItemSlotType UItemSlotWidget::GetSlotType() const
{
	return slotType;
}

int32 UItemSlotWidget::GetSlotNum() const
{
	return slotNum;
}

FItemData UItemSlotWidget::GetItemData() const
{
	return itemData;
}

void UItemSlotWidget::SetSlotType(const EItemSlotType Type)
{
	this->slotType = Type;
}

void UItemSlotWidget::SetItemData(const FItemData Data)
{
	this->itemData = Data;
}

FReply UItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		//플레이어가 조작 불가능이면 리턴
		ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetOwningPlayer());
		if (rpgController)
		{
			if (!rpgController->GetControl()) return reply.NativeReply;
		}

		//우클릭 시, 아이템 사용 및 장착
		switch (itemData.itemType)
		{
		case EItemType::Consum:
			UsedItem();
			break;
		case EItemType::Access_Ear: case EItemType::Access_Neck: case EItemType::Access_Ring:
		case EItemType::Equip_Body: case EItemType::Equip_Hand: case EItemType::Equip_Head: case EItemType::Equip_Leg: case EItemType::Equip_Shoulder: case EItemType::Weapone:
			ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
			if (player)
			{
				//공격 중일시 장비템 장착 해제 불가
				if (rpgController && rpgController->GetAttack())
				{
					return reply.NativeReply;
				}
				if (slotType == EItemSlotType::Inventory)
				{
					if (bArt && (itemData.itemType == EItemType::Access_Ear || itemData.itemType == EItemType::Access_Ring))
					{
						player->ArtEquipmentItem(itemData, slotNum);
					}
					else
					{
						player->EquipMentItem(itemData, slotNum);
					}
				}
				else if (slotType != EItemSlotType::Quick)
				{
					player->ReleaseItem(itemData, slotNum);
				}
			}
			break;
		}
	}

	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr && itemData.itemName != "")
	{
		UItemDragDropOperation* oper = NewObject<UItemDragDropOperation>();
		OutOperation = oper;
		oper->itemData = itemData;

		//드래그 비주얼이 가리키는 값이 없으면 로드
		if (dragVisual==nullptr)
		{
			dragVisual = LoadClass<UItemSlotWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/WBP_ItemSlot.WBP_ItemSlot_C'"));
		}
		//드래그 비주얼이 가리키는 값이 있는지 확인 후 위젯을 만들고 오퍼에 대입
		if (dragVisual)
		{
			UItemSlotWidget* visual = CreateWidget<UItemSlotWidget>(GetWorld(), dragVisual);
			if (visual)
			{
				visual->itemData = itemData;
				visual->slotNum = slotNum;
				visual->slotType = slotType;
				oper->DefaultDragVisual = visual;
			}
		}
	}
}

bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UE_LOG(LogTemp, Warning, TEXT("drop"));
	if (InOperation)
	{
		UItemDragDropOperation* oper = Cast<UItemDragDropOperation>(InOperation);
		UItemSlotWidget* visual = Cast<UItemSlotWidget>(InOperation->DefaultDragVisual);
		if (oper && visual)
		{
			switch (slotType)
			{
				case EItemSlotType::Inventory:
					if (visual->slotType == EItemSlotType::Inventory)
					{
						invenswapDelegate.ExecuteIfBound(visual->slotNum, slotNum);
					}
					break;

				case EItemSlotType::Quick:
					if (oper->itemData.itemType == EItemType::Consum)
					{
						if (visual->slotType == EItemSlotType::Inventory)
						{
							invenToQuick.ExecuteIfBound(visual->itemData, slotNum);
						}
						else if (visual->slotType == EItemSlotType::Quick)
						{
							quickSwapDelegate.ExecuteIfBound(visual->slotNum, slotNum);
						}
					}
					break;
			}
		}

		return true;
	}
	return false;
}

void UItemSlotWidget::UsedItem()
{
	if (!itemData.bReady) return;

	ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
	if (player)
	{
		player->UsedItem(itemData.itemName);
	}
}