// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_ConvertBoolenValue.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTTaskNode_ConvertBoolenValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_ConvertBoolenValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector boolenKey;
};
