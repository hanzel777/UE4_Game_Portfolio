// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "RPGPlayer.h"
#include "MainUI.h"
#include <Blueprint\WidgetTree.h>

void UEquipment::NativeConstruct()
{
	TArray<UWidget*> widgets;
 	WidgetTree->GetAllWidgets(widgets);

	UItemSlotWidget* slot = nullptr;
	for (auto widget : widgets)
	{
		slot = Cast<UItemSlotWidget>(widget);
		if (slot)
		{
			slots.Add(slot);
			slot->GetSlotType();
			switch (slot->GetSlotType())
			{
				case EItemSlotType::Access_Ear:
					earSlots.Add(slot);
					break;
				case EItemSlotType::Access_Ring:
					ringSlots.Add(slot);
				case EItemSlotType::Access_Neck:
					neckSlot.Add(slot);
					break;
				case EItemSlotType::Equip_Head:
					headSlot.Add(slot);
					break;
				case EItemSlotType::Equip_Shoulder:
					shoulSlot.Add(slot);
					break;
				case EItemSlotType::Equip_Body:
					bodySlot.Add(slot);
					break;
				case EItemSlotType::Equip_Leg:
					legSlot.Add(slot);
					break;
				case EItemSlotType::Equip_Hand:
					handSlot.Add(slot);
					break;
				case EItemSlotType::Weapone:
					weapSlot.Add(slot);
					break;
			}
			slot = nullptr;
		}
	}
}

void UEquipment::InitEquipment(UDataTable* table)
{
	weaponeTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/DataTables/ItemTable/WeaponeTable.WeaponeTable'"));

	equipTable = table;

	TArray<FItemData*> itemDatas;
	equipTable->GetAllRows<FItemData>("error", itemDatas);

	strength = 0;
	defence = 0;
	runSpeed = 0;

	ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
	if (player)
	{
		strength += player->GetStrength();
		defence += player->GetDefence();
		runSpeed += player->GetRunSpeed();
	}

	//아이템 테이블의 슬롯 이름(Ear2, Neck etc...)을 받아온다.
	TArray<FName> itemKinds;
	itemKinds = equipTable->GetRowNames();

	for (auto itemKind : itemKinds)
	{
		FItemData* itemData = equipTable->FindRow<FItemData>(itemKind, "Error Ear1");
		strength += itemData->addStrength;
		defence += itemData->addDefence;
		runSpeed += itemData->addRunSpeed;

		if (itemKind == TEXT("Ear1"))
		{
			bEar1 = true;
			earSlots[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Ear1"));
		}
		else if (itemKind == TEXT("Ear2"))
		{
			bEar2 = true;
			earSlots[1]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Ear2"));
		}
		else if (itemKind == TEXT("Ring1"))
		{
			bRing1 = true;
			ringSlots[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Ring1"));
		}
		else if (itemKind == TEXT("Ring2"))
		{
			bRing2 = true;
			ringSlots[1]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Ring2"));
		}
		else if (itemKind == TEXT("Neck")) 
		{
			bNeck = true;
			neckSlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Neck"));
		}
		else if (itemKind == TEXT("Head"))
		{
			bHead = true;
			headSlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error head"));
		}
		else if (itemKind == TEXT("Shoul"))
		{
			bShoul = true;
			shoulSlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error shoul"));
		}
		else if (itemKind == TEXT("Body")) 
		{
			bBody = true;
			bodySlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error body"));
		}
		else if (itemKind == TEXT("Leg"))
		{
			bLeg = true;
			legSlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Leg"));
		}
		else if (itemKind == TEXT("Hand")) 
		{
			bHand = true;
			handSlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Hand"));
		}
		else if (itemKind == TEXT("Weapone"))
		{
			bWeap = true;
			weapSlot[0]->SetItemData(*equipTable->FindRow<FItemData>(itemKind, "Error Weapone"));
			SetupWeapone(weapSlot[0]->GetItemData().itemName);
		}
	}

	player->SetStrength(strength);
	player->SetDefence(defence);
	player->SetRunSpeed(runSpeed);
}

bool UEquipment::SetSlot(FItemData& data, const int32 slotNum)
{
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == data.itemName) return false;
	}

	bool bFullOrEmpty;
	switch (data.itemType)
	{
		case EItemType::Access_Ear:
			bFullOrEmpty = (bEar1 == bEar2);
			for (int i = 0; i < earSlots.Num(); i++)
			{
				if (!bFullOrEmpty && earSlots[i]->GetItemData().itemName != "") continue;
				Swap(earSlots[i], data, slotNum);
				if (i == 0)
				{
					equipTable->AddRow(TEXT("Ear1"), earSlots[i]->GetItemData());
					bEar1 = true;
				}
				if (i != 0)
				{
					equipTable->AddRow(TEXT("Ear2"), earSlots[i]->GetItemData());
					bEar2 = true;
				}
				return true;
			}
			break;

		case EItemType::Access_Ring:
			bFullOrEmpty = (bRing1 == bRing2); 
			for (int i = 0; i < ringSlots.Num(); i++)
			{
				if (!bFullOrEmpty && ringSlots[i]->GetItemData().itemName != "") continue;
				Swap(ringSlots[i], data, slotNum);
				if (i == 0) 
				{
					equipTable->AddRow(TEXT("Ring1"), ringSlots[i]->GetItemData());
					bRing1 = true;
				}
				if (i != 0)
				{
					equipTable->AddRow(TEXT("Ring2"), ringSlots[i]->GetItemData());
					bRing2 = true;
				}
				return true;
			}
			break;

		case EItemType::Access_Neck:
			Swap(neckSlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Neck"), neckSlot[0]->GetItemData());
			bNeck = true;
			return true;
			break;

		case EItemType::Equip_Head:
			Swap(headSlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Head"), headSlot[0]->GetItemData());
			bHead = true;
			return true;
			break;

		case EItemType::Equip_Shoulder:
			Swap(shoulSlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Shoul"), shoulSlot[0]->GetItemData());
			bShoul = true;
			return true;
			break;

		case EItemType::Equip_Body:
			Swap(bodySlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Body"), bodySlot[0]->GetItemData());
			bBody = true;
			return true;
			break;

		case EItemType::Equip_Leg:
			Swap(legSlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Leg"), legSlot[0]->GetItemData());
			bLeg = true;
			return true;
			break;

		case EItemType::Equip_Hand:
			Swap(handSlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Hand"), handSlot[0]->GetItemData());
			bHand = true;
			return true;
			break;

		case EItemType::Weapone:
			Swap(weapSlot[0], data, slotNum);
			equipTable->AddRow(TEXT("Weapone"), weapSlot[0]->GetItemData());
			bWeap = true;
			SetupWeapone(weapSlot[0]->GetItemData().itemName);
			return true;
			break;
	}
	return false;
}

void UEquipment::ArtSetSlot(FItemData& data, const int32 slotNum)
{
	for (auto slot : slots)
	{
		if (slot->GetItemData().itemName == data.itemName) return;
	}

	if (data.itemType == EItemType::Access_Ear)
	{
		Swap(earSlots[1], data, slotNum);
		equipTable->AddRow(TEXT("Ear2"), earSlots[1]->GetItemData());
		bEar2 = true;
	}
	else if (data.itemType == EItemType::Access_Ring)
	{
		Swap(ringSlots[1], data, slotNum);
		equipTable->AddRow(TEXT("Ring2"), ringSlots[1]->GetItemData());
		bRing2 = true;
	}
}

void UEquipment::Swap(UItemSlotWidget* slot, FItemData& data, const int32 slotNum)
{
	FItemData temp = slot->GetItemData();
	slot->SetItemData(data);

	ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
	if (player)
	{
		player->AddStrength(slot->GetItemData().addStrength);
		strength = player->GetStrength();

		defence = player->GetDefence() + slot->GetItemData().addDefence;
		runSpeed = player->GetRunSpeed() + slot->GetItemData().addRunSpeed;

		player->SetDefence(defence);
		player->SetRunSpeed(runSpeed);
		if (temp.itemName == "")
		{
			player->GetMainUI()->GetInventory()->RemoveItem(slotNum);
		}
		else
		{
			player->GetMainUI()->GetInventory()->InsertItem(temp, slotNum);
		}
	}
}

void UEquipment::SetupWeapone(const FString weaponeName)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupWeapone"));

	if (weaponeTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set weapone %s"), *weaponeName);

		TArray<FWeaponeData*> weaponeDatas;
		weaponeTable->GetAllRows<FWeaponeData>("", weaponeDatas);

		for (auto weaponeData : weaponeDatas)
		{
			if (weaponeData->weaponeName == weaponeName)
			{
				ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
				if (player)
				{
					if (weaponeData->skeletalMesh != nullptr)
						player->SetSK_Weapone(weaponeData->skeletalMesh, weaponeData->location, weaponeData->rotation, weaponeData->scale);
					//if(weaponeData->staticMesh!=nullptr)
				}
			}
		}
	}
}

void UEquipment::RemoveItem(const int32 slotNum)
{
	UE_LOG(LogTemp, Warning, TEXT("UEquipment::RemoveItem"));
	
	//장착 슬롯의 수만큼 반복하여 매개변수로 받은 슬롯과 일치하는 슬롯을 찾아 장착 슬롯 배열의 인덱스를 찾는다.
	uint8 num = 0;
	for (uint8 i = 0; i < slots.Num(); i++)
	{
		if (slots[i]->GetSlotNum() == slotNum)
		{
			num = i;
			break;
		}
	}

	ARPGPlayer* player = Cast<ARPGPlayer>(GetOwningPlayerPawn());
	if (player)
	{
		switch (slots[slotNum]->GetSlotType())
		{
		case EItemSlotType::Access_Ear:
			if (earSlots[0]->itemData.itemName == slots[num]->itemData.itemName)
			{
				UE_LOG(LogTemp, Warning, TEXT("UEquipment::Ear1"));
				equipTable->RemoveRow(FName(TEXT("Ear1")));
				bEar1 = false;
			}
			else
			{
				equipTable->RemoveRow(FName(TEXT("Ear2")));
				bEar2 = false;
			}
			break;

		case EItemSlotType::Access_Ring:
			if (ringSlots[0]->itemData.itemName == slots[num]->itemData.itemName)
			{
				equipTable->RemoveRow(FName(TEXT("Ring1")));
				bRing1 = false;
			}
			else {
				equipTable->RemoveRow(FName(TEXT("Ring2")));
				bRing2 = false;
			}
			break;

		case EItemSlotType::Access_Neck:
			equipTable->RemoveRow(FName(TEXT("Neck")));
			bNeck = false;
			break;
		case EItemSlotType::Equip_Head:
			equipTable->RemoveRow(FName(TEXT("Head")));
			bHead = false;
			break;
		case EItemSlotType::Equip_Shoulder:
			equipTable->RemoveRow(FName(TEXT("Shoul")));
			bShoul = false;
			break;
		case EItemSlotType::Equip_Body:
			equipTable->RemoveRow(FName(TEXT("Body")));
			bBody = false;
			break;
		case EItemSlotType::Equip_Leg:
			equipTable->RemoveRow(FName(TEXT("Leg")));
			bLeg = false;
			break;
		case EItemSlotType::Equip_Hand:
			equipTable->RemoveRow(FName(TEXT("Hand")));
			bHand = false;
			break;
		case EItemSlotType::Weapone:
			equipTable->RemoveRow(FName(TEXT("Weapone")));
			bWeap = false;
			player->ReleasWeapone();	
			break;
		}
		player->SubStrength(slots[num]->GetItemData().addStrength);
		strength = player->GetStrength();

		defence = player->GetDefence() - slots[num]->GetItemData().addDefence;
		runSpeed = player->GetRunSpeed() - slots[num]->GetItemData().addRunSpeed;
		player->SetDefence(defence);
		player->SetRunSpeed(runSpeed);
		slots[num]->itemData.RemoveData();
	}
}

void UEquipment::SetStrength(const float str)
{
	strength = str;
}