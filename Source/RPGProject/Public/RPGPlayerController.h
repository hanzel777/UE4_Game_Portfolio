// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MousePlayerController.h"
#include "SkillData.h"
#include "ItemData.h"
#include "RPGPlayerController.generated.h"

DECLARE_DELEGATE_OneParam(FUseItemKey, const FString)

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ARPGPlayerController : public AMousePlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	//러쉬 쿨타임
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CoolTime)
	float rushDelayTime = 6;

	bool bStiffImmunity;
	bool bHitImmunity;

	TArray<FSkillData*> skillDatas;
	TArray<FItemData*> itemDatas;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = UI)
	TSubclassOf<class UMainUI> mainWidget;

	//메인 UI에서 바인드 받는 델리게이트
	FUseItemKey useItemDelegate;

	UFUNCTION(BlueprintCallable)
	void OnStiff(AActor* OverlappedActor, const float speed);
	UFUNCTION(BlueprintCallable)
	void OnHit(AActor* OverlappedActor, const float speed, const float upSpeed);

protected:

private:
	bool bRushReady = true;
	float rushCurrentTime = 0;

	//스킬 0~7
	void OnSKill(FSkillData* skillData);
	void OnSkill_0();
	void OnSkill_1();
	void OnSkill_2();
	void OnSkill_3();
	void OnSkill_4();
	void OnSkill_5();
	void OnSkill_6();
	void OnSkill_7();

	void ReSkill(FSkillData* skillData);
	void ReSkill_0();
	void ReSkill_1();
	void ReSkill_2();
	void ReSkill_3();
	void ReSkill_4();
	void ReSkill_5();
	void ReSkill_6();
	void ReSkill_7();

	void OnItem(FItemData* itemData);
	void OnItem_0();
	void OnItem_1();
	void OnItem_2();
	void OnItem_3();
	void OnItem_4();
	void OnItem_5();
	void OnItem_6();
	void OnItem_7();

	//러쉬
	void OnRush();

	//쿨타임 계산
	void CollTime(float DeltaTime);
};
