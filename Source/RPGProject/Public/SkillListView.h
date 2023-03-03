// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillListView.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillListView : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UListView* listview;

private:
	//��ų ����� ��ų�� Ŭ�� ���� �� ��µǰ� ���ε�
	void ItemClick(UObject* MyListView);

	//��ų ������ ������Ʈ�� ������ List�� AddItem
	void SetSkillList();

	class UDataTable* skillTable;
};
