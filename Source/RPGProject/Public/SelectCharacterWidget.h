// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USelectCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString characterClass;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString nickName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString level;

	int32 slotNum;

	UFUNCTION()
	void OnSelect();

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* button;

	virtual void NativeConstruct() override;

public:
	void SetSlotNum(const int32 num);

	int32 GetSlotNum() const;

	void SetNickName(const FString name);
	void SetCharacterClass(const FString className);
	void SetLevel(const int32 level);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetColor();
};
