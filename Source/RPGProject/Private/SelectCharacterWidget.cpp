// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterWidget.h"
#include "Components/Button.h"
#include "SelectPlayerController.h"

void USelectCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	button = Cast<UButton>(GetWidgetFromName("Button"));

	if (button)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSelect %d"), slotNum);

		button->OnClicked.AddDynamic(this, &USelectCharacterWidget::OnSelect);
	}
}

void USelectCharacterWidget::OnSelect()
{
	ASelectPlayerController* controller = Cast< ASelectPlayerController>(GetOwningPlayer());
	if (controller)
	{
		controller->OnSelected(slotNum);
	}
}

void USelectCharacterWidget::SetSlotNum(const int32 num)
{
	slotNum = num;
}

int32 USelectCharacterWidget::GetSlotNum() const
{
	return slotNum;
}

void USelectCharacterWidget::SetNickName(const FString name)
{
	if (name.IsEmpty() || name == "None") return;
	nickName = name;
}
void USelectCharacterWidget::SetCharacterClass(const FString className)
{
	characterClass = className;
}
void USelectCharacterWidget::SetLevel(const int32 num)
{
	if (num <= 0) return;
	level = FString::Printf(TEXT("Lv. %d"), num);
}