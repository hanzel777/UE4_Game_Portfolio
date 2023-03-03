// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "RPGPlayer.h"

ItemData::ItemData()
{
}

ItemData::~ItemData()
{
}

void ItemData::UsedItem(ARPGPlayer* player, const FString name)
{
	UE_LOG(LogTemp, Warning, TEXT("Used item %s"), *name);
	if (name == "HpPotion")
	{
		ItemData::HpPotion(player);
	}
}

void ItemData::HpPotion(ARPGPlayer* player)
{
	UE_LOG(LogTemp, Warning, TEXT("ItemData::HpPotion"));

	float hp = player->GetCurrentHp() + (player->GetMaxHp() * 0.3f);
	if (hp > player->GetMaxHp()) hp = player->GetMaxHp();
	player->SetCurrentHp(hp);
}


//이상 없으면 지우기
//void FItemData::RemoveData()
//{
//	itemName = "";
//	itemCount = 0;
//	delayTime = 0;
//	currentTime = 0;
//	bReady = true;
//	itemType;
//	tex = baseTex;
//}
//
//void FItemData::SetItemTexture(const FString path)
//{
//	tex = LoadObject<UTexture2D>(nullptr, *path);
//}