// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_EnemyAttackTask.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTTask_EnemyAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_EnemyAttackTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
