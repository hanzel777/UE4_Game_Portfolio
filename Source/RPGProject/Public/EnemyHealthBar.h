// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
	class UProgressBar* healthBar;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, BindWidget = true))
	class UTextBlock* monsterName;

public:
	virtual void NativeConstruct() override;

	void UpdateHpPercent(const float percent);
	void MonsterNameVisibility(const bool isVisible, const FString name);
};
