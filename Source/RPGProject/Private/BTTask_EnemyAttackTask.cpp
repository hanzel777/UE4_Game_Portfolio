// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyAttackTask.h"
#include <BehaviorTree\BlackboardComponent.h>

UBTTask_EnemyAttackTask::UBTTask_EnemyAttackTask()
{
	NodeName = TEXT("FinishedEnemyAttack");
}

EBTNodeResult::Type UBTTask_EnemyAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	return EBTNodeResult::Succeeded;
}