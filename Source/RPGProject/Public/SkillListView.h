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
	//스킬 목록의 스킬을 클릭 했을 시 출력되게 바인드
	void ItemClick(UObject* MyListView);

	//스킬 데이터 오브젝트를 생성해 List에 AddItem
	void SetSkillList();

	class UDataTable* skillTable;
};
