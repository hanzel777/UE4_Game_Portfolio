// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatePlayerController.h"
#include "CreateUI.h"

ACreatePlayerController::ACreatePlayerController()
{
	ConstructorHelpers::FClassFinder<UCreateUI> temp(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_CreateUI.WBP_CreateUI_C'"));
	if (temp.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("createWidget"));
		createWidget = temp.Class;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> table(TEXT("DataTable'/Game/DataTables/ClassDataTable.ClassDataTable'"));
	if (table.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("classTable"));

		classTable = table.Object;
	}
}

void ACreatePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	if (createWidget)
	{
		createUI = CreateWidget<UCreateUI>(this, createWidget);
		createUI->AddToViewport();
		if(classTable) createUI->SetTable(classTable);
	}
}