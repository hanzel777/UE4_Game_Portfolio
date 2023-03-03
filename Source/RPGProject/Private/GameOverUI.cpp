// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	titleButton = Cast<UButton>(GetWidgetFromName(TEXT("TitleButton")));
	if (titleButton)titleButton->OnClicked.AddDynamic(this, &UGameOverUI::OnClickTitleButton);


	exitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
	if (exitButton)exitButton->OnClicked.AddDynamic(this, &UGameOverUI::OnClickExitButton);
}

void UGameOverUI::OnClickTitleButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TitleMap");
}

void UGameOverUI::OnClickExitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}