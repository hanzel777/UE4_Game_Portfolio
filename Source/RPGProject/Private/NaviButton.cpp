// Fill out your copyright notice in the Description page of Project Settings.


#include "NaviButton.h"
#include "Components/Button.h"
#include "RPGGameInstance.h"
#include <Kismet\GameplayStatics.h>
#include <Kismet\KismetStringLibrary.h>

void UNaviButton::NativeConstruct()
{
	Super::NativeConstruct();

	startButton = Cast<UButton>(GetWidgetFromName("StartButton"));
	if (startButton)
	{
		startButton->OnClicked.AddDynamic(this, &UNaviButton::OnStartButton);
	}
}

void UNaviButton::OnStartButton()
{
	URPGGameInstance* gameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()));
	gameInstance->testNumber += 1;
	UGameplayStatics::OpenLevel(GetWorld(), "SelectLevel");
}