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

	//�÷��̾� ���� �ް� ��Ʈ�ѷ��� �޾ƿ� RPG�÷��̾�� ����ȯ
	//�����ϸ� ��ų ������ �迭�� ��ų ���� ������ŭ �ʱ�ȭ
	ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(GetOwningPlayerPawn()->GetController());
	if (rpgController)
	{
		rpgController->skillDatas.Init(nullptr, skills.Num());
	}

	//��ų ���� �迭 ������ ��ų ���� ��ȣ�� �´� ��ų ������ �迭 �ε����� ��ų �����͸� ��ų ������ ��ų �������� �ּ� ������ �ʱ�ȭ
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

	//��ų ���̺��� ��� ��ų ������ �޾ƿ´�.
	TArray<FSkillData*> skillDatas;
	skillTable->GetAllRows<FSkillData>("", skillDatas);

	//��ų ���̺��� ���� ��ȣ(�̸�)�� �޾ƿ´�.
	TArray<FName> skillIndexes;
	skillIndexes = skillTable->GetRowNames();

	for (int32 i = 0; i < skillIndexes.Num(); i++)
	{
		//��ų ������ ���̺��� �����͸� ��ų ���Կ� �Ѱ��ش�.
		if (skillIndexes[i].ToString() != "0" && FCString::Atoi(*skillIndexes[i].ToString()) == 0) continue;
		skills[FCString::Atoi(*skillIndexes[i].ToString())]->skillData = *skillDatas[i];
		//�����÷ο� ����
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