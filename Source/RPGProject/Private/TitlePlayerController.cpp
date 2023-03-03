// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "NaviButton.h"

ATitlePlayerController::ATitlePlayerController()
{
	bShowMouseCursor = true;

	ConstructorHelpers::FClassFinder<UUserWidget> title(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_TitleUI.WBP_TitleUI_C'"));
	if (title.Succeeded())
	{
		titleWidget = title.Class;
	}
}

void ATitlePlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());

	titleUI = CreateWidget<UUserWidget>(this, titleWidget);
	titleUI->AddToViewport();
}