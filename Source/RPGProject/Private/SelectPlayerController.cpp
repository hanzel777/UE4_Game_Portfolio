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

		//유저 데이터 테이블의 행 이름을 순회
		for (auto rowName : userTable->GetRowNames())
		{
			//행이름을 정수형으로 형변환
			uint8 index = FCString::Atoi(*rowName.ToString());
			//유저 테이블에서 행이름과 맞는 데이터를 불러옴
			FPlayerData* playerData = userTable->FindRow<FPlayerData>(rowName, "");
			//열거형을 문자열으로 바꾸기 위한 문자열 변수 선언 및 초기화
			FString className;
			UEnum* enumptr;
			//스위치를 통해 기본 타입을 찾고 그에 맞는 열거형을 문자열로 바꿔 대입
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

			//행의 이름이 레벨에 대입시킬 폰의 개수보다 크면 작동시키지 않음
			if (selectPawns.Num() > index)
			{
				//폰의 번호와 행의 번호가 일치하면 바로 폰에 데이터를 전달
				if (selectPawns[index]->GetPawnNum() == index)
				{
					selectPawns[index]->SetMeshAndAnim(playerData->classData.mesh, playerData->classData.selectAnim, playerData->nickName.ToString(), className, playerData->level);
				}
				//일치하지 않으면 일치하는 폰을 찾고 폰에 데이터를 전달
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

//게임 인스턴스의 현재 플레이어 데이터 변수에 선택된 플레이어 데이터 대입
void ASelectPlayerController::OnSelected(const int32 num)
{
	for (auto select : selectPawns)
	{
		if (select->GetPawnNum() == num)
		{
			URPGGameInstance* instance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(this));
			if (instance)
			{
				//현재 플레이어 데이터가 있다면 플레이어 데이터의 슬롯 번호를 찾고 글자색을 원래색으로 돌려준다.
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
							//선택한 플레이어가 현재 게임 인스턴스에 있는 현재 플레이어와 같다면 즉, 선택한 플레이어를 또 선택한다면 아무일도 없이 리턴
							if (select->GetPawnNum() == ind)
							{
								return;
							}
							for (auto selectPawn : selectPawns)
							{
								//기존에 선택되어 있던 폰을 찾아서 컬러를 원래 컬러로 돌려줌
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
		//현재 플레이어 데이터가 있다면 유저 테이블에서 닉네임이 같은 데이터르 찾아 초기화
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
		// 플레이어 데이터가 없다면
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Delete because not exists Selected Character"));
		}
	}
}