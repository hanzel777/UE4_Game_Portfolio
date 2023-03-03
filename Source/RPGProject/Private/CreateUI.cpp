// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateUI.h"
#include "SelectPawn.h"
#include "RPGGameInstance.h"
#include <Components\Button.h>
#include <Components\EditableTextBox.h>
#include <Kismet\GameplayStatics.h>

void UCreateUI::NativeConstruct()
{
	Super::NativeConstruct();

	createButton = Cast<UButton>(GetWidgetFromName("CreateButton"));
	if (createButton)
	{
		createButton->OnClicked.AddDynamic(this, &UCreateUI::OnCreateButton);
	}

	nextButton = Cast<UButton>(GetWidgetFromName("NextButton"));
	if (nextButton)
	{
		nextButton->OnClicked.AddDynamic(this, &UCreateUI::OnNextButton);
	}

	reverseButton = Cast<UButton>(GetWidgetFromName("ReverseButton"));
	if (reverseButton)
	{
		reverseButton->OnClicked.AddDynamic(this, &UCreateUI::OnReverseButton);
	}

	editTextBox = Cast<UEditableTextBox>(GetWidgetFromName("EditTextBox"));
	if (editTextBox)
	{
		editTextBox->OnTextChanged.AddDynamic(this, &UCreateUI::OnChangeNickName);
	}
	pawn = Cast<ASelectPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ASelectPawn::StaticClass()));
}

void UCreateUI::OnCreateButton()
{
	if (nickName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No NickName"));
		return;
	}

	URPGGameInstance* instance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(GetOwningPlayer()));
	if (instance)
	{
		TArray<FName> indexes;
		indexes = instance->userTable->GetRowNames();

		//닉네임이 중복되면 return
		for (auto index : indexes)
		{
			FPlayerData* playerData = instance->userTable->FindRow<FPlayerData>(index, "");
			if (playerData->nickName == *nickName)
			{
				UE_LOG(LogTemp, Warning, TEXT("This NickName already exist"));

				return;
			}
		}


		for (int i = 0; i < indexes.Num(); i++)
		{
			for (auto index : indexes)
			{
				if (i == FCString::Atoi(*index.ToString()))
				{
					FPlayerData* playerData = instance->userTable->FindRow<FPlayerData>(index, "");
					if (playerData && playerData->nickName == "")
					{
						playerData->classData = *classDatas[currentIndex];
						playerData->level = 1;
						playerData->nickName = FName(*nickName);
						playerData->currentHp = classDatas[currentIndex]->MaxHp;
						playerData->currentMp = classDatas[currentIndex]->MaxMp;
						instance->userTable->AddRow(index, *playerData);
						UGameplayStatics::OpenLevel(GetWorld(), "SelectLevel");
						return;
					}
					break;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Can't Creat is Full Character"));
}

void UCreateUI::OnNextButton()
{
	currentIndex++;
	if (currentIndex >= classDatas.Num()) currentIndex = 0;
	ShowCharacter(classDatas[currentIndex]);
}

void UCreateUI::OnReverseButton()
{
	currentIndex--;
	if (currentIndex < 0) currentIndex = classDatas.Num() - 1;
	ShowCharacter(classDatas[currentIndex]);
}

void UCreateUI::OnChangeNickName(const FText& Text)
{
	nickName = Text.ToString();
}

void UCreateUI::SetTable(UDataTable* table)
{
	classTable = table;
	classTable->GetAllRows<FClassData>("", classDatas);

	currentIndex = 0;
	ShowCharacter(classDatas[currentIndex]);
}

void UCreateUI::ShowCharacter(const FClassData* classData)
{
	FString classBaseName = "";
	FString classDetailName = "";
	const UEnum* classBase = FindObject<UEnum>(ANY_PACKAGE, TEXT("EClassType"), true);
	classBaseName = classBase->GetNameStringByValue((int64)classData->classType);
	switch (classData->classType)
	{
		UEnum* classDetail;
	case EClassType::Warrior:
		classDetail = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWarriorClassType"), true);
		if (classDetail)
		{
			classDetailName = classDetail->GetNameStringByValue((int64)classData->warrior);
		}
		break;
	case EClassType::Wizard:
		classDetail = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWizardClassType"), true);
		if (classDetail)
		{
			classDetailName = classDetail->GetNameStringByValue((int64)classData->wizard);
		}
		break;
	case EClassType::Archer:
		classDetail = FindObject<UEnum>(ANY_PACKAGE, TEXT("EArcherClassType"), true);
		if (classDetail)
		{
			classDetailName = classDetail->GetNameStringByValue((int64)classData->archer);
		}
		break;
	case EClassType::Fighter:
		classDetail = FindObject<UEnum>(ANY_PACKAGE, TEXT("EFighterClassType"), true);
		if (classDetail)
		{
			classDetailName = classDetail->GetNameStringByValue((int64)classData->fighter);
		}
		break;
	}

	pawn->SetMeshAndAnim(classData->mesh, classData->selectAnim, classBaseName, classDetailName, 0);
}