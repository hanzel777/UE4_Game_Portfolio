// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"
#include "SkillDragAndDropOperation.h"
#include "SkillSlot.h"
#include <Blueprint\WidgetBlueprintLibrary.h>

void USkillWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	USkillDataObject* object = Cast<USkillDataObject>(ListItemObject);
	if (object)
	{
		skillData = object->skillData;
	}
}

FReply USkillWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//���콺 ���� Ŭ��
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		//��Ŭ�� ��, ���콺 �巡�� ����
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void USkillWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//��ų �ý�ó�� ������� �ʰ� ���۰� ������� ��
	if (OutOperation == nullptr && skillData.Texture && skillData.bSkillReady)
	{
		//���ۿ� �������� USkillDragAndDropOperationŬ������ ���� ������Ʈ�� �����ϰ� ���۰� ���� �� ���ۿ� ��ų ������ ����
		USkillDragAndDropOperation* oper = NewObject<USkillDragAndDropOperation>();
		OutOperation = oper;
		oper->skillData = skillData;

		//�巡�� ���־��� Ŭ������ ã�ƿ´�
		dragVisual = LoadClass<USkillWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/WBP_Skill_Icon_List.WBP_Skill_Icon_List_C'"));

		//�巡�� ���־��� ���� ��
		if (dragVisual)
		{
			//�巡�� ���־��� �޸𸮿� �÷��ְ� ��ų �������� �ý�ó�� ���� �� ���ۿ� �巡�� ���־� ����
			USkillWidget* visual = CreateWidget<USkillWidget>(GetWorld(), dragVisual);
			visual->skillData.Texture = skillData.Texture;
			visual->skill_Slot_Num = skill_Slot_Num;
			oper->DefaultDragVisual = visual;

			//Ÿ���� ������ ��� ���Կ� �ִ� ��ų ������ ����
			if (type == ESkillWidgetType::Slot)
			{
				skillData.RemoveData();
				removeSkillSlotDeleGate.ExecuteIfBound(skill_Slot_Num);
			}
		}
	}
}

bool USkillWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USkillDragAndDropOperation* oper = Cast<USkillDragAndDropOperation>(InOperation);
	if (oper && type==ESkillWidgetType::Slot)
	{
		if (skillData.bSkillReady)
		{
			setSkillSlotDeleGate.ExecuteIfBound(oper->skillData, skillData, skill_Slot_Num);
		}
		return true;
	}
	else
	{
		return false;
	}
}

int32 USkillWidget::GetSkillSlotNum() const
{
	return skill_Slot_Num;
}