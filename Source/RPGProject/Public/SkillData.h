// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine\DataTable.h>
#include "SkillData.generated.h"

/**
 * 
 */
class RPGPROJECT_API SkillData
{
public:
	SkillData();
	~SkillData();
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Normal,
	Combo,
	Holding,
	Casting,
	Charging
};

USTRUCT(Atomic, BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString skillName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		UTexture2D* Texture;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float coolTime;
	UPROPERTY(BlueprintReadOnly)
		float currentTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool bSkillReady = true;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		ESkillType skillType;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float spendMp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool bStiffImmunity;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool bHitImmunity;

	void RemoveData();
};