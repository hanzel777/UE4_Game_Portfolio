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

	//마우스 좌측 클릭
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		//좌클릭 시, 마우스 드래그 검출
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void USkillWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//스킬 택스처가 비어있지 않고 오퍼가 비어있을 시
	if (OutOperation == nullptr && skillData.Texture && skillData.bSkillReady)
	{
		//오퍼에 대입해줄 USkillDragAndDropOperation클래스의 오퍼 오브젝트를 생성하고 오퍼가 참조 후 오퍼에 스킬 데이터 대입
		USkillDragAndDropOperation* oper = NewObject<USkillDragAndDropOperation>();
		OutOperation = oper;
		oper->skillData = skillData;

		//드래그 비주얼을 클래스를 찾아온다
		dragVisual = LoadClass<USkillWidget>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprints/WBP_Skill_Icon_List.WBP_Skill_Icon_List_C'"));

		//드래그 비주얼이 있을 시
		if (dragVisual)
		{
			//드래그 비주얼을 메모리에 올려주고 스킬 데이터의 택스처를 대입 후 오퍼에 드래그 비주얼에 대입
			USkillWidget* visual = CreateWidget<USkillWidget>(GetWorld(), dragVisual);
			visual->skillData.Texture = skillData.Texture;
			visual->skill_Slot_Num = skill_Slot_Num;
			oper->DefaultDragVisual = visual;

			//타입이 슬롯인 경우 슬롯에 있는 스킬 정보를 제거
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