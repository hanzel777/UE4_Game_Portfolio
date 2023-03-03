// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.h"
#include "Equipment.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEquipment : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<UItemSlotWidget*> slots;

	TArray<UItemSlotWidget*> neckSlot;
	TArray<UItemSlotWidget*> earSlots;
	TArray<UItemSlotWidget*> ringSlots;

	TArray<UItemSlotWidget*> headSlot;
	TArray<UItemSlotWidget*> shoulSlot;
	TArray<UItemSlotWidget*> bodySlot;
	TArray<UItemSlotWidget*> legSlot;
	TArray<UItemSlotWidget*> handSlot;
	TArray<UItemSlotWidget*> weapSlot;


	bool bNeck = false;
	bool bEar1 = false;
	bool bEar2 = false;
	bool bRing1 = false;
	bool bRing2 = false;

	bool bHead = false;
	bool bShoul = false;
	bool bBody = false;
	bool bLeg = false;
	bool bHand = false;
	bool bWeap = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float strength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float defence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float runSpeed;

	void Swap(UItemSlotWidget* slots, FItemData& data, const int32 slotNum);
	void SetupWeapone(const FString weaponeName);

	UDataTable* equipTable;
	UDataTable* weaponeTable;

protected:
	virtual void NativeConstruct() override;
public:
	void InitEquipment(UDataTable* table);

	bool SetSlot(FItemData& data, const int32 slotNum);
	void ArtSetSlot(FItemData& data, const int32 slotNum);

	void RemoveItem(const int32 slotNum);

	void SetStrength(const float str);
};
