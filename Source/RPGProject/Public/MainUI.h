// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.h"
#include "SkillListView.h"
#include "SkillSlot.h"
#include "QuickSlot.h"
#include "Equipment.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UInventory* inventory;
	USkillListView* skillListView;
	USkillSlot* skillSlot;
	UQuickSlot* quickSlot;
	UEquipment* equipment;

	void InvenCoolTime(float InDeltaTime);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UFUNCTION(BlueprintCallable)
	UInventory* GetInventory() const;
	USkillListView* GetSkillListView() const;
	USkillSlot* GetSkillSlot() const;
	UQuickSlot* GetQuickSlot() const;
	UEquipment* GetEquipment() const;

	UFUNCTION()
	void UsedItem(const FString name);

	UFUNCTION()
	void SetQuickSlot(const FItemData itemData, const int32 slotNum);

	void GainItem(const FItemData data);
};
