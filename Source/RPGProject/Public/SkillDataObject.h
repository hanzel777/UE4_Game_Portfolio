// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillData.h"
#include "SkillDataObject.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillDataObject : public UObject
{
	GENERATED_BODY()
	

public:
	FSkillData skillData;
};
