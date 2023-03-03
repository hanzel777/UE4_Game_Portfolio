// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

URPGGameInstance::URPGGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> user(TEXT("DataTable'/Game/DataTables/UserDataTable.UserDataTable'"));
	if (user.Succeeded())
	{
		userTable = user.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> level(TEXT("DataTable'/Game/DataTables/LevelTable.LevelTable'"));
	if (level.Succeeded())
	{
		levelTable = level.Object;
	}

}

void URPGGameInstance::UpdatedCuurentPlayerDataInUserTable()
{
	if (currentPlayerData == nullptr) return;
	if (userTable == nullptr)return;

	TArray<FName> rowNames;
	rowNames = userTable->GetRowNames();

	for (auto rowName : rowNames)
	{
		FPlayerData* playerData = userTable->FindRow<FPlayerData>(rowName, "");
		if (playerData && playerData->nickName == currentPlayerData->nickName)
		{
			userTable->AddRow(rowName, *currentPlayerData);
			return;
		}
	}
}

void URPGGameInstance::OpenNextLevel()
{
	if (currentPlayerData == nullptr) return;
	if (levelTable == nullptr) return;

	TArray<FLevelData*> levelDatas;
	levelTable->GetAllRows<FLevelData>("", levelDatas);

	for (auto levelData : levelDatas)
	{
		if (levelData->level_Index == currentPlayerData->level)
		{
			UGameplayStatics::OpenLevel(GetWorld(), levelData->level_Name);
			return;
		}
	}
}