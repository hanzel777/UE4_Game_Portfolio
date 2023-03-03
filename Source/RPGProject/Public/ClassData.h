// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine\DataTable.h>
#include "ClassData.generated.h"

/**
 * 
 */
class RPGPROJECT_API ClassData
{
public:
	ClassData();
	~ClassData();
};

UENUM(BlueprintType)
enum class EClassType : uint8
{
	None,
	Warrior,
	Wizard,
	Archer,
	Fighter
};

UENUM(BlueprintType)
enum class EWarriorClassType : uint8
{
	None,
	Berserker,
	Axeman,
	Knight,
	Cyborg
};

UENUM(BlueprintType)
enum class EWizardClassType : uint8
{
	None,
	Sorceress,
	Summoner
};

UENUM(BlueprintType)
enum class EArcherClassType : uint8
{
	None,
	Bowman,
	Rifleman
};

UENUM(BlueprintType)
enum class EFighterClassType : uint8
{
	None,
	StreetFighter,
	Boxer
};

USTRUCT(Atomic, BlueprintType)
struct FClassData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EClassType classType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWarriorClassType warrior;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWizardClassType wizard;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EArcherClassType archer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EFighterClassType fighter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMesh* mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> selectAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> basicAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxMp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JumpDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JumpSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Strength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Defence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* skillDataTable;

	void Remove()
	{
		classType = EClassType::None;
		warrior = EWarriorClassType::None;
		wizard = EWizardClassType::None;
		archer = EArcherClassType::None;
		fighter = EFighterClassType::None;
		mesh = nullptr;
		selectAnim = nullptr;
		basicAnim = nullptr;
		MaxHp = 0;
		MaxMp = 0;
		RunSpeed = 0;
		JumpDistance = 0;
		Strength = 0;
		Defence = 0;
		skillDataTable = nullptr;
	}
};