// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelData.generated.h"

/**
 * 
 */
class RPGPROJECT_API LevelData
{
public:
	LevelData();
	~LevelData();
};

USTRUCT(Atomic, BlueprintType)
struct FLevelData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 level_Index;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FName level_Name;
};