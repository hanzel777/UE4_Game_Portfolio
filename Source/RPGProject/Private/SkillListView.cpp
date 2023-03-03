// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillListView.h"
#include "SkillWidget.h"
#include <Components\ListView.h>

void USkillListView::NativeConstruct()
{
	Super::NativeConstruct();

	listview = Cast<UListView>(GetWidgetFromName(TEXT("ListView")));

	if (listview)
	{
		listview->OnItemClicked().AddUObject(this, &USkillListView::ItemClick);

		SetSkillList();
	}
}

void USkillListView::ItemClick(UObject* MyListView)
{
	USkillDataObject* skill = Cast<USkillDataObject>(MyListView);
}


void USkillListView::SetSkillList()
{
	skillTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/Game/DataTables/RoboSkillBaseTable.RoboSkillBaseTable'"));

	if (skillTable)
	{
		TArray<FSkillData*> skillDatas;
		skillTable->GetAllRows<FSkillData>("", skillDatas);

		for (auto skillData : skillDatas)
		{
			USkillDataObject* skillObj = NewObject<USkillDataObject>();
			skillObj->skillData = *skillData;
			listview->AddItem(skillObj);
		}
	}

	//USkillDataObject* skill0 = NewObject<USkillDataObject>();
	//skill0->skillData.skillName = "Skill0";
	//skill0->skillData.skillType = ESkillType::Normal;
	//skill0->skillData.coolTime = 5;
	//skill0->skillData.currentTime = 0;
	//skill0->skillData.bSkillReady = true;
	//skill0->skillData.Texture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Textures/frame_0_delay-0_01s.frame_0_delay-0_01s'"));
	//listview->AddItem(skill0);

	//USkillDataObject* skill1 = NewObject<USkillDataObject>();
	//skill1->skillData.skillName = "Skill1";
	//skill1->skillData.skillType = ESkillType::Normal;
	//skill1->skillData.coolTime = 3;
	//skill1->skillData.currentTime = 0;
	//skill1->skillData.bSkillReady = true;
	//skill1->skillData.Texture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/Textures/test.test'"));
	//listview->AddItem(skill1);

	//USkillDataObject* skill2 = NewObject<USkillDataObject>();
	//skill2->skillData.skillName = "Skill2";
	//skill2->skillData.skillType = ESkillType::Normal;
	//skill2->skillData.coolTime = 3;
	//skill2->skillData.currentTime = 0;
	//skill2->skillData.bSkillReady = true;
	//skill2->skillData.Texture;
	//listview->AddItem(skill2);

	//USkillDataObject* skill3 = NewObject<USkillDataObject>();
	//skill3->skillData.skillName = "Skill3";
	//skill3->skillData.skillType = ESkillType::Normal;
	//skill3->skillData.coolTime = 3;
	//skill3->skillData.currentTime = 0;
	//skill3->skillData.bSkillReady = true;
	//skill3->skillData.Texture;
	//listview->AddItem(skill3);

	//USkillDataObject* skill4 = NewObject<USkillDataObject>();
	//skill4->skillData.skillName = "Skill4";
	//skill4->skillData.skillType = ESkillType::Normal;
	//skill4->skillData.coolTime = 3;
	//skill4->skillData.currentTime = 0;
	//skill4->skillData.bSkillReady = true;
	//skill4->skillData.Texture;
	//listview->AddItem(skill4);

	//USkillDataObject* skill5 = NewObject<USkillDataObject>();
	//skill5->skillData.skillName = "Skill5";
	//skill5->skillData.skillType = ESkillType::Normal;
	//skill5->skillData.coolTime = 3;
	//skill5->skillData.currentTime = 0;
	//skill5->skillData.bSkillReady = true;
	//skill5->skillData.Texture;
	//listview->AddItem(skill5);

	//USkillDataObject* skill6 = NewObject<USkillDataObject>();
	//skill6->skillData.skillName = "Skill6";
	//skill6->skillData.skillType = ESkillType::Normal;
	//skill6->skillData.coolTime = 3;
	//skill6->skillData.currentTime = 0;
	//skill6->skillData.bSkillReady = true;
	//skill6->skillData.Texture;
	//listview->AddItem(skill6);

	//USkillDataObject* skill7 = NewObject<USkillDataObject>();
	//skill7->skillData.skillName = "Skill7";
	//skill7->skillData.skillType = ESkillType::Normal;
	//skill7->skillData.coolTime = 3;
	//skill7->skillData.currentTime = 0;
	//skill7->skillData.bSkillReady = true;
	//skill7->skillData.Texture;
	//listview->AddItem(skill7);
}