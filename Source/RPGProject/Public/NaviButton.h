// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NaviButton.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UNaviButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* startButton;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* endButton;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* optionButton;

	UFUNCTION()
	void OnStartButton();
};
