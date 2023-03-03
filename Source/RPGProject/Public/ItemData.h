// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine\DataTable.h"
#include "ItemData.generated.h"

/**
 * 
 */
class RPGPROJECT_API ItemData
{
public:
	ItemData();
	~ItemData();

	static void UsedItem(class ARPGPlayer* player, const FString name);

private:
	static void HpPotion(class ARPGPlayer* player);
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consum,
	Equip_Head,
	Equip_Shoulder,
	Equip_Body,
	Equip_Leg,
	Equip_Hand,
	Access_Neck,
	Access_Ear,
	Access_Ring,
	Weapone
};

USTRUCT(Atomic, BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

private:
	UTexture2D* baseTex = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Textures/Textured-Black-Tile-BY6001G-.Textured-Black-Tile-BY6001G-'"));
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString itemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 itemCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float delayTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bReady;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType itemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* tex = baseTex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float addStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float addDefence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float addRunSpeed;

	void RemoveData()
	{
		itemName = "";
		itemCount = 0;
		delayTime = 0;
		currentTime = 0;
		bReady = true;
		itemType;
		tex = baseTex;
	}
	void SetItemTexture(const FString path)
	{
		tex = LoadObject<UTexture2D>(nullptr, *path);
	}
};