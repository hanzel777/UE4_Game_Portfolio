// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassData.h"
#include "WeaponeData.h"
#include "PlayerData.generated.h"

/**
 * 
 */
class RPGPROJECT_API PlayerData
{
public:
	PlayerData();
	~PlayerData();
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName nickName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 level;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float currentHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float currentMp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FClassData classData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UDataTable* inventoryTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UDataTable* quickSlotTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UDataTable* equipSlotTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UDataTable* skillSlotTable;

	void Remove()
	{
		nickName = FName(TEXT(""));
		level = 0;
		currentHp = 0;
		currentMp = 0;
		classData.Remove();
		inventoryTable->EmptyTable();
		quickSlotTable->EmptyTable();
		equipSlotTable->EmptyTable();
		skillSlotTable->EmptyTable();
	}
};