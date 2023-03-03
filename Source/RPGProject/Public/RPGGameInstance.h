// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelData.h"
#include "PlayerData.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URPGGameInstance();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 testNumber;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	FString testMesh = TEXT("SkeletalMesh'/Game/Test/FemailMesh.FemailMesh'");

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	FString testAnim = TEXT("AnimBlueprint'/Game/Animation/ABP_SelectAnim.ABP_SelectAnim_C'");

	UDataTable* levelTable;

	UDataTable* userTable;

	FPlayerData* currentPlayerData;

	void UpdatedCuurentPlayerDataInUserTable();

	void OpenNextLevel();
};
