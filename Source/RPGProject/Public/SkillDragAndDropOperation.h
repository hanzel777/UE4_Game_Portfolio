// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SkillData.h"
#include "SkillDragAndDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USkillDragAndDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FSkillData skillData;
};
