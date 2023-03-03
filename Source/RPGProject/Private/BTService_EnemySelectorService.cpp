// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_EnemySelectorService.h"
#include "EnemyBaseCharacter.h"
#include "RPGPlayer.h"
#include <AIController.h>
#include <BehaviorTree\BlackboardComponent.h>
#include <Kismet\GameplayStatics.h>

UBTService_EnemySelectorService::UBTService_EnemySelectorService()
{
	NodeName = TEXT("EnemySelectorService");
}

void UBTService_EnemySelectorService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemy == nullptr) return;
	if (enemy->bDeath) return;

	CaculateDistanceAndUpdateKeyValue(OwnerComp);

	if (enemy->bRecognize != OwnerComp.GetBlackboardComponent()->GetValueAsBool(bRecognizeKey.SelectedKeyName))
	{
		enemy->bRecognize = OwnerComp.GetBlackboardComponent()->GetValueAsBool(bRecognizeKey.SelectedKeyName);
		OwnerComp.RestartTree();
	}
	if (enemy->bAttack == false && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bAttackKey.SelectedKeyName) == true)
	{
		enemy->bAttack = true;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(bNotCalculateKey.SelectedKeyName, true);
		OwnerComp.RestartTree();
	}
	else if (enemy->bAttack == true && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bAttackKey.SelectedKeyName) == false)
	{
		enemy->bAttack = false;
	}
	if (enemy->bHit == false && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bHitKey.SelectedKeyName) == true)
	{
		enemy->bHit = true;
		enemy->bAttack = false;
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(bAttackKey.SelectedKeyName, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(bNotCalculateKey.SelectedKeyName, true);
		OwnerComp.RestartTree();
	}
	else if (enemy->bHit == true && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bHitKey.SelectedKeyName) == false)
	{
		enemy->bHit = false;
	}
	if (enemy->bDeath == false && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bDeathKey.SelectedKeyName) == true)
	{
		enemy->bDeath = true;
		OwnerComp.RestartTree();
	}
}

void UBTService_EnemySelectorService::CaculateDistanceAndUpdateKeyValue(UBehaviorTreeComponent& OwnerComp)
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemy == nullptr) return;

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(bNotCalculateKey.SelectedKeyName)) return;

	//월드내에 플레이어들을 찾아냄
	TArray<AActor*> targets;
	UGameplayStatics::GetAllActorsOfClass(enemy->GetWorld(), ARPGPlayer::StaticClass(), targets);
	
	//플레이어들과의 거리를 계산 및 블랙보드 변수 수정
	for (auto target : targets)
	{
		//인식 중일 때, 인식하고 있는 플레이어와  다른 플레이어라면 리턴
		if (enemy->bRecognize && OwnerComp.GetBlackboardComponent()->GetValueAsObject(tagetObjKey.SelectedKeyName) != target) return;
		
		float distance;
		distance = FVector::Distance(enemy->GetActorLocation(), target->GetActorLocation());

		//공격 중이 아니고 공격 범위 안에 들어왔고 플레이어를 보고 있다면 공격
		if (distance <= enemy->attackRange && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bSeePlayerKey.SelectedKeyName))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bAttackKey.SelectedKeyName, true);
		}
		//인식 중일 때, 추격 범위 밖으로 나가면 인식 못함
		else if (distance > enemy->chaseRange && enemy->bRecognize)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bRecognizeKey.SelectedKeyName, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(tagetObjKey.SelectedKeyName, nullptr);
		}
		//인식 중이 아닐 때, 인식 범위 안으로 들어오면 인식
		else if (distance <= enemy->recognizeRange && !enemy->bRecognize)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bAggroKey.SelectedKeyName, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bRecognizeKey.SelectedKeyName, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(tagetObjKey.SelectedKeyName, target);
		}
	}
}