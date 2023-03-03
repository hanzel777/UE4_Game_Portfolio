// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClassData.h"
#include "CreateUI.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UCreateUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTable(class UDataTable*);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* createButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* nextButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* reverseButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* editTextBox;

	UFUNCTION()
	void OnCreateButton();
	UFUNCTION()
	void OnNextButton();
	UFUNCTION()
	void OnReverseButton();
	UFUNCTION()
	void OnChangeNickName(const FText& Text);

	void ShowCharacter(const FClassData* classData);

	FString nickName;

	class ASelectPawn* pawn;

	class UDataTable* classTable;

	TArray<FClassData*> classDatas;

	int32 currentIndex = 0;
};
