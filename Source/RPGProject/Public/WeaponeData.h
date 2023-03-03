// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine\DataTable.h"
#include "ClassData.h"
#include "WeaponeData.generated.h"

/**
 * 
 */
class RPGPROJECT_API WeaponeData
{
public:
	WeaponeData();
	~WeaponeData();
};

USTRUCT(Atomic, BlueprintType)
struct FWeaponeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EWarriorClassType warrior;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EWizardClassType wizard;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EArcherClassType archer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EFighterClassType fighter;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString weaponeName;	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float strength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* itemTex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USkeletalMesh* skeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMesh* staticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FVector location;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FRotator rotation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FVector scale = FVector(1, 1, 1);

	void Remove()
	{
		warrior = EWarriorClassType::None;
		wizard = EWizardClassType::None;
		archer = EArcherClassType::None;
		fighter = EFighterClassType::None;
		weaponeName = "";
		itemTex = nullptr;
		skeletalMesh = nullptr;
		staticMesh = nullptr;
		location = FVector::ZeroVector;
		rotation = FRotator::ZeroRotator;
		scale = FVector::ZeroVector;
	}
};