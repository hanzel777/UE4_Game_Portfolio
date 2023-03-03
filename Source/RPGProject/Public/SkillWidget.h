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
	//스킬 위젯 타입
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
		ESkillWidgetType type;

	//슬롯 타입의 스킬 위젯을 위한 변수
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	int32 skill_Slot_Num;

protected:
	//스킬 이미지
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* skill_Img;

	//드래그 앤랍에 사용할 위젯
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		TSubclassOf<USkillWidget> dragVisual;

	//ListView의 스킬 정보 받아오기
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	//마우스 클릭 이벤트 >> 마우스 좌클릭시 드래그 검출
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//드래그 검출될 시, 이벤트 실행
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//드랍 검출 시, 이벤트 실행
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	//스킬의 데이터를 갖는 구조체
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FSkillData skillData;

	//스킬 슬롯 함수를 실행할 델리 게이트 변수 선언
	FSetSkillSlotDeleGate setSkillSlotDeleGate;
	FRemoveSkillSlotDeleGate removeSkillSlotDeleGate;

	//스킬 슬롯 넘버 받아오기
	int32 GetSkillSlotNum() const;
};
