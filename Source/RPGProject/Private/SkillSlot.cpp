// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlot.h"
#include "SkillWidget.h"
#include "RPGPlayerController.h"
#include <Blueprint/WidgetTree.h>

void USkillSlot::NativeConstruct()
{
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USkillWidget* skill = nullptr;
	for (auto widget : widgets)
	{
		skill = Cast<USkillWidget>(widget);
		if (skill)
		{
			skills.Add(skill);
		}
	}

	//플레이어 폰을 받고 컨트롤러를 받아와 RPG플레이어로 형변환
	//성공하면 스킬 데이터 배열을 스킬 슬롯 갯수만큼 초기화
	ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetOwningPlayerPawn()->GetController());
	if (rpgController)
	{
		rpgController->skillDatas.Init(nullptr, skills.Num());
	}

	//스킬 슬롯 배열 인자의 스킬 슬롯 번호에 맞는 스킬 데이터 배열 인덱스의 스킬 데이터를 스킬 슬롯의 스킬 데이터의 주소 값으로 초기화
	for (auto skillWidget : skills)
	{
		rpgController->skillDatas[skillWidget->GetSkillSlotNum()] = &skillWidget->skillData;
		skillWidget->setSkillSlotDeleGate.BindUObject(this, &USkillSlot::SetSkill);
		skillWidget->removeSkillSlotDeleGate.BindUObject(this, &USkillSlot::RemoveTableSlot);
	}
}

void USkillSlot::InitSkillSlot(UDataTable* table)
{
	skillTable = table;

	//스킬 테이블의 모든 스킬 정보를 받아온다.
	TArray<FSkillData*> skillDatas;
	skillTable->GetAllRows<FSkillData>("", skillDatas);

	//스킬 테이블의 슬롯 번호(이름)을 받아온다.
	TArray<FName> skillIndexes;
	skillIndexes = skillTable->GetRowNames();

	for (int32 i = 0; i < skillIndexes.Num(); i++)
	{
		//스킬 데이터 테이블의 데이터를 스킬 슬롯에 넘겨준다.
		if (skillIndexes[i].ToString() != "0" && FCString::Atoi(*skillIndexes[i].ToString()) == 0) continue;
		skills[FCString::Atoi(*skillIndexes[i].ToString())]->skillData = *skillDatas[i];
		//오버플로우 방지
		if (i >= skills.Num())break;
	}
}

void USkillSlot::SetSkill(const FSkillData operSkillData, FSkillData& skillData, const int32 slotNum)
{
	for (auto skill : skills)
	{
		if (operSkillData.skillName == skill->skillData.skillName)
		{
			return;
		}
	}
	
	skillData = operSkillData;
	skillTable->AddRow(*FString::FromInt(slotNum), skillData);
}

void USkillSlot::RemoveTableSlot(const int32 slotNum)
{
	skillTable->RemoveRow(*FString::FromInt(slotNum));

}