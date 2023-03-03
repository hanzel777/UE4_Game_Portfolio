// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDataObject.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SkillWidget.generated.h"

DECLARE_DELEGATE_ThreeParams(FSetSkillSlotDeleGate, const FSkillData, FSkillData&, const int32)
DECLARE_DELEGATE_OneParam(FRemoveSkillSlotDeleGate, const int32)

UENUM()
enum class ESkillWidgetType : uint8
{
	List,
	Slot
};
/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

private:
	//��ų ���� Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
		ESkillWidgetType type;

	//���� Ÿ���� ��ų ������ ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	int32 skill_Slot_Num;

protected:
	//��ų �̹���
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* skill_Img;

	//�巡�� �ض��� ����� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<USkillWidget> dragVisual;

	//ListView�� ��ų ���� �޾ƿ���
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	//���콺 Ŭ�� �̺�Ʈ >> ���콺 ��Ŭ���� �巡�� ����
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//�巡�� ����� ��, �̺�Ʈ ����
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//��� ���� ��, �̺�Ʈ ����
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	//��ų�� �����͸� ���� ����ü
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FSkillData skillData;

	//��ų ���� �Լ��� ������ ���� ����Ʈ ���� ����
	FSetSkillSlotDeleGate setSkillSlotDeleGate;
	FRemoveSkillSlotDeleGate removeSkillSlotDeleGate;

	//��ų ���� �ѹ� �޾ƿ���
	int32 GetSkillSlotNum() const;
};
