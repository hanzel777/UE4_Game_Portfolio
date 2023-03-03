// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillData.h"
#include "SkillSlot.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<class USkillWidget*> skills;

	UDataTable* skillTable;

protected:
	virtual void NativeConstruct() override;
public:
	void InitSkillSlot(UDataTable* table);

	UFUNCTION()
	void SetSkill(const FSkillData operSkillData, FSkillData& skillData, const int32 slotNum);
	UFUNCTION()
	void RemoveTableSlot(const int32 slotNum);
};
