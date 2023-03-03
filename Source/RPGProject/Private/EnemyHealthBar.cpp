// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UEnemyHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	healthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));

	monsterName = Cast<UTextBlock>(GetWidgetFromName(TEXT("MonsterName")));
}

void UEnemyHealthBar::UpdateHpPercent(const float percent)
{
	if (healthBar)
	{
		healthBar->SetPercent(percent);
	}
}

void UEnemyHealthBar::MonsterNameVisibility(const bool isVisible, const FString name)
{
	if (isVisible)
	{
		monsterName->SetText(FText::FromString(name));
	}
	else
	{
		monsterName->SetText(FText());
	}
}