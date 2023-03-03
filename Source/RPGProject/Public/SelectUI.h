// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectUI.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USelectUI : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UButton* startButton;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UButton* createButton;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
	class UButton* deleteButton;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnStart();
	UFUNCTION()
	void OnCreate();
	UFUNCTION()
	void OnDelete();
};
