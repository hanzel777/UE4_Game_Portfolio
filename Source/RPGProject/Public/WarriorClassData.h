// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassData.h"
#include "WarriorClassData.generated.h"

/**
 * 
 */
class RPGPROJECT_API WarriorClassData
{
public:
	WarriorClassData();
	~WarriorClassData();
};

USTRUCT(Atomic, BlueprintType)
struct FWarriorClassData : public FClassData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWarriorClassType warriorClassType;

	FWarriorClassData()
	{
		classType = EClassType::Warrior;
	}
};