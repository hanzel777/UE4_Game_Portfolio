// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta=(BindWidget))
	class UButton* titleButton;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* exitButton;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickTitleButton();
	UFUNCTION()
	void OnClickExitButton();
};
