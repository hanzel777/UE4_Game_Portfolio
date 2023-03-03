// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectUI.h"
#include "SelectPlayerController.h"
#include "RPGGameInstance.h"
#include <Components\Button.h>
#include <Kismet\GamePlayStatics.h>

void USelectUI::NativeConstruct()
{
	Super::NativeConstruct();

	startButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	if (startButton)
	{
		startButton->OnClicked.AddDynamic(this, &USelectUI::OnStart);
	}

	createButton = Cast<UButton>(GetWidgetFromName(TEXT("CreateButton")));
	if (createButton)
	{
		createButton->OnClicked.AddDynamic(this, &USelectUI::OnCreate);
	}

	deleteButton = Cast<UButton>(GetWidgetFromName(TEXT("DeleteButton")));
	if (deleteButton)
	{
		deleteButton->OnClicked.AddDynamic(this, &USelectUI::OnDelete);
	}
}

void USelectUI::OnStart()
{
	URPGGameInstance* rpgGameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()));
	if (rpgGameInstance)
	{
		if(rpgGameInstance->currentPlayerData == nullptr) return;

		rpgGameInstance->OpenNextLevel();
	}
}

void USelectUI::OnCreate()
{
	UGameplayStatics::OpenLevel(this, "CreateCharacter");
}

void USelectUI::OnDelete()
{
	ASelectPlayerController* playerContoller = Cast< ASelectPlayerController>(GetOwningPlayer());
	if (playerContoller)
	{
		playerContoller->OnDeleted();
	}
}