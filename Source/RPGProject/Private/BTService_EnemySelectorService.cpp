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

	//���峻�� �÷��̾���� ã�Ƴ�
	TArray<AActor*> targets;
	UGameplayStatics::GetAllActorsOfClass(enemy->GetWorld(), ARPGPlayer::StaticClass(), targets);
	
	//�÷��̾����� �Ÿ��� ��� �� ������ ���� ����
	for (auto target : targets)
	{
		//�ν� ���� ��, �ν��ϰ� �ִ� �÷��̾��  �ٸ� �÷��̾��� ����
		if (enemy->bRecognize && OwnerComp.GetBlackboardComponent()->GetValueAsObject(tagetObjKey.SelectedKeyName) != target) return;
		
		float distance;
		distance = FVector::Distance(enemy->GetActorLocation(), target->GetActorLocation());

		//���� ���� �ƴϰ� ���� ���� �ȿ� ���԰� �÷��̾ ���� �ִٸ� ����
		if (distance <= enemy->attackRange && OwnerComp.GetBlackboardComponent()->GetValueAsBool(bSeePlayerKey.SelectedKeyName))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bAttackKey.SelectedKeyName, true);
		}
		//�ν� ���� ��, �߰� ���� ������ ������ �ν� ����
		else if (distance > enemy->chaseRange && enemy->bRecognize)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bRecognizeKey.SelectedKeyName, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(tagetObjKey.SelectedKeyName, nullptr);
		}
		//�ν� ���� �ƴ� ��, �ν� ���� ������ ������ �ν�
		else if (distance <= enemy->recognizeRange && !enemy->bRecognize)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bAggroKey.SelectedKeyName, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(bRecognizeKey.SelectedKeyName, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(tagetObjKey.SelectedKeyName, target);
		}
	}
}