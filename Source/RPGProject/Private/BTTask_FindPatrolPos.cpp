// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "EnemyBaseCharacter.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include <BehaviorTree\BlackboardComponent.h>

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemy == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyBaseCharacter miss"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* nav = UNavigationSystemV1::GetNavigationSystem(OwnerComp.GetAIOwner()->GetWorld());
	if (nav == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UNavigationSystemV1 miss"));
		return EBTNodeResult::Failed;
	}

	FVector origin = enemy->originPos;
	float radius = enemy->patrolRadius;
	FNavLocation newLocation;
	if (nav->GetRandomReachablePointInRadius(origin, radius, newLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), newLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}