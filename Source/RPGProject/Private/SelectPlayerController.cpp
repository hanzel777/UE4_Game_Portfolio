// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPlayerController.h"
#include "SelectUI.h"
#include "SelectPawn.h"
#include "RPGGameInstance.h"
#include "EngineUtils.h"
#include <Kismet\GamePlayStatics.h>

ASelectPlayerController::ASelectPlayerController()
{
	bShowMouseCursor = true;

	ConstructorHelpers::FClassFinder<USelectUI> tempSelect(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_SelectUI.WBP_SelectUI_C'"));
	if (tempSelect.Succeeded())
	{
		selectWidget = tempSelect.Class;
	}
}

void ASelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	URPGGameInstance* rpgGameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (rpgGameInstance)
	{
		rpgGameInstance->currentPlayerData = nullptr;
		for (TActorIterator<ASelectPawn> it(GetWorld()); it; ++it)
		{
			ASelectPawn* selectPawn = *it;
			selectPawns.Add(*it);
		}


		UDataTable* userTable = rpgGameInstance->userTable;

		//���� ������ ���̺��� �� �̸��� ��ȸ
		for (auto rowName : userTable->GetRowNames())
		{
			//���̸��� ���������� ����ȯ
			uint8 index = FCString::Atoi(*rowName.ToString());
			//���� ���̺��� ���̸��� �´� �����͸� �ҷ���
			FPlayerData* playerData = userTable->FindRow<FPlayerData>(rowName, "");
			//�������� ���ڿ����� �ٲٱ� ���� ���ڿ� ���� ���� �� �ʱ�ȭ
			FString className;
			UEnum* enumptr;
			//����ġ�� ���� �⺻ Ÿ���� ã�� �׿� �´� �������� ���ڿ��� �ٲ� ����
			switch (playerData->classData.classType)
			{
			case EClassType::Warrior:
				enumptr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWarriorClassType"), true);
				if (enumptr)
				{
					className = enumptr->GetNameStringByValue((int64)playerData->classData.warrior);
				}
				break;
			case EClassType::Wizard:
				enumptr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWizardClassType"), true);
				if (enumptr)
				{
					className = enumptr->GetNameStringByValue((int64)playerData->classData.wizard);
				}
				break;
			case EClassType::Archer:
				enumptr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EArcherClassType"), true);
				if (enumptr)
				{
					className = enumptr->GetNameStringByValue((int64)playerData->classData.archer);
				}
				break;
			case EClassType::Fighter:
				enumptr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EFighterClassType"), true);
				if (enumptr)
				{
					className = enumptr->GetNameStringByValue((int64)playerData->classData.fighter);
				}
				break;
			}

			//���� �̸��� ������ ���Խ�ų ���� �������� ũ�� �۵���Ű�� ����
			if (selectPawns.Num() > index)
			{
				//���� ��ȣ�� ���� ��ȣ�� ��ġ�ϸ� �ٷ� ���� �����͸� ����
				if (selectPawns[index]->GetPawnNum() == index)
				{
					selectPawns[index]->SetMeshAndAnim(playerData->classData.mesh, playerData->classData.selectAnim, playerData->nickName.ToString(), className, playerData->level);
				}
				//��ġ���� ������ ��ġ�ϴ� ���� ã�� ���� �����͸� ����
				else
				{
					for (auto select : selectPawns)
					{
						if (select->GetPawnNum() == index)
						{
							select->SetMeshAndAnim(playerData->classData.mesh, playerData->classData.selectAnim, playerData->nickName.ToString(), className, playerData->level);
							break;
						}
					}
				}
			}
		}
	}

	if (selectWidget)
	{
		selectUI = CreateWidget<USelectUI>(this, selectWidget);
		if (selectUI)
		{
			selectUI->AddToViewport();
		}
	}

	FInputModeUIOnly inputMode;
	SetInputMode(inputMode);
}

//���� �ν��Ͻ��� ���� �÷��̾� ������ ������ ���õ� �÷��̾� ������ ����
void ASelectPlayerController::OnSelected(const int32 num)
{
	for (auto select : selectPawns)
	{
		if (select->GetPawnNum() == num)
		{
			URPGGameInstance* instance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(this));
			if (instance)
			{
				//���� �÷��̾� �����Ͱ� �ִٸ� �÷��̾� �������� ���� ��ȣ�� ã�� ���ڻ��� ���������� �����ش�.
				if(instance->currentPlayerData)
				{
					TArray<FName> indexes;
					indexes = instance->userTable->GetRowNames();

					for (auto index : indexes)
					{
						FPlayerData* playerData = instance->userTable->FindRow<FPlayerData>(index, "");
						if (playerData->nickName == instance->currentPlayerData->nickName)
						{
							int32 ind = FCString::Atoi(*index.ToString());
							//������ �÷��̾ ���� ���� �ν��Ͻ��� �ִ� ���� �÷��̾�� ���ٸ� ��, ������ �÷��̾ �� �����Ѵٸ� �ƹ��ϵ� ���� ����
							if (select->GetPawnNum() == ind)
							{
								return;
							}
							for (auto selectPawn : selectPawns)
							{
								//������ ���õǾ� �ִ� ���� ã�Ƽ� �÷��� ���� �÷��� ������
								if (selectPawn->GetPawnNum() == ind)
								{
									selectPawn->SetSlotColor();
								}
							}
						}
					}
				}
				instance->currentPlayerData = instance->userTable->FindRow<FPlayerData>(*FString::FromInt(num), "");
				if (instance->currentPlayerData->nickName.IsNone())
				{
					UE_LOG(LogTemp, Warning, TEXT("None"));
					instance->currentPlayerData = nullptr;
				}
			}
			select->OnSelected();
			break;
		}
	}
}

void ASelectPlayerController::OnDeleted()
{
	URPGGameInstance* instance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (instance)
	{
		//���� �÷��̾� �����Ͱ� �ִٸ� ���� ���̺��� �г����� ���� �����͸� ã�� �ʱ�ȭ
		if (instance->currentPlayerData != nullptr && !instance->currentPlayerData->nickName.IsNone())
		{
			TArray<FName> indexes;
			indexes = instance->userTable->GetRowNames();

			for (auto index : indexes)
			{
				FPlayerData playerData = *instance->userTable->FindRow<FPlayerData>(index, "");
				if (playerData.nickName == instance->currentPlayerData->nickName)
				{
					playerData.Remove();
					instance->userTable->AddRow(index, playerData);
					instance->currentPlayerData = nullptr;
					UGameplayStatics::OpenLevel(this, "SelectLevel");
					return;
				}
			}
		}
		// �÷��̾� �����Ͱ� ���ٸ�
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Delete because not exists Selected Character"));
		}
	}
}