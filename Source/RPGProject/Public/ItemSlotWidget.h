// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemData.h"
#include "ItemSlotWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FInvenSwapDelegate, const int32, const int32)
DECLARE_DELEGATE_TwoParams(FInvenToQuick, const FItemData, const int32)
DECLARE_DELEGATE_TwoParams(FQucikSwapDelegate, const int32, const int32)

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemSlotType : uint8
{
	Inventory,
	Quick,
	Equip_Head,
	Equip_Shoulder,
	Equip_Body,
	Equip_Leg,
	Equip_Hand,
	Access_Neck,
	Access_Ear,
	Access_Ring,
	Weapone
};

UCLASS()
class RPGPROJECT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	EItemSlotType slotType;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 slotNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UImage* img;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FItemData itemData;
	//인벤토리 스왑 델리게이트
	FInvenSwapDelegate invenswapDelegate;

	//인벤토리에서 퀵 슬롯으로
	FInvenToQuick invenToQuick;
	//퀵슬롯 스왑 델리게이트
	FQucikSwapDelegate quickSwapDelegate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<UItemSlotWidget> dragVisual;

	EItemSlotType GetSlotType() const;

	int32 GetSlotNum() const;

	FItemData GetItemData() const;

	void SetSlotType(const EItemSlotType slotType);

	void SetItemData(const FItemData itemData);

	void UsedItem();

	bool bArt = false;
};
