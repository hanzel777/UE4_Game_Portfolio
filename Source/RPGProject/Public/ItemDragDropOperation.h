// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemData.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FItemData itemData;
};
