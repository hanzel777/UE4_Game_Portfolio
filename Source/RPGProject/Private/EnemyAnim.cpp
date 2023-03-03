// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "EnemyBaseCharacter.h"
#include "EnemyBaseProjectile.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		speed = FVector::DotProduct(pawn->GetActorForwardVector(), pawn->GetVelocity());
		direction = FVector::DotProduct(pawn->GetActorRightVector(), pawn->GetVelocity());
	}
}

void UEnemyAnim::GetRandomAggroAnimAsset(UAnimationAsset*& animAsset) const
{
	if (0 < aggroAnimAssets.Num())
	{
		const int index = FMath::RandRange(0, aggroAnimAssets.Num()-1);
		animAsset = aggroAnimAssets[index];
	}
}
void UEnemyAnim::GetRandomAttackAnimAsset(UAnimationAsset*& animAsset) const
{
	if (0 < attackAnimAssets.Num())
	{
		const int index = FMath::RandRange(0, attackAnimAssets.Num() - 1);
		animAsset = attackAnimAssets[index];
	}
}
void UEnemyAnim::GetRandomHitAnimAsset(UAnimationAsset*& animAsset) const
{
	if (0 < hitAnimAssets.Num())
	{
		const int index = FMath::RandRange(0, hitAnimAssets.Num() - 1);
		animAsset = hitAnimAssets[index];
	}
}
void UEnemyAnim::GetRandomDeathAnimAsset(UAnimationAsset*& animAsset) const
{
	if (0 < deathAnimAssets.Num())
	{
		const int index = FMath::RandRange(0, deathAnimAssets.Num() - 1);
		animAsset = deathAnimAssets[index];
	}
}

void UEnemyAnim::AnimNotify_OnAttackCollision()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		enemy->GetBoxComponent()->SetCollisionProfileName(TEXT("EnemyAttack"));
	}
}

void UEnemyAnim::AnimNotify_OnAttackCollisisonEnd()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		enemy->GetBoxComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void UEnemyAnim::AnimNotify_OnSpawnProjectile()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		AEnemyBaseProjectile* projectileInstance = enemy->GetWorld()->SpawnActor<AEnemyBaseProjectile>(projectile, enemy->GetBoxComponent()->GetComponentTransform());
		if(projectileInstance) projectileInstance->SetDamage(enemy->GetAttackDamage());
	}
}

void UEnemyAnim::AnimNotify_OnAggroTaskEnd()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());
		if (controller)
		{
			controller->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bAggroKey, false);
			controller->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bNotCalculateDistanceKey, false);
			UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(controller->GetBrainComponent());
			if (bt) bt->RestartTree();
		}
	}
}

void UEnemyAnim::AnimNotify_OnAttackTaskEnd()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());
		if (controller)
		{
			controller->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bAttackKey, false);
			controller->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bNotCalculateDistanceKey, false);
			UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(controller->GetBrainComponent());
			if (bt) bt->RestartTree();
		}
	}
}

void UEnemyAnim::AnimNotify_OnHitTaskEnd()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());
		if (controller)
		{
			controller->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bNotCalculateDistanceKey, false);
			UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(controller->GetBrainComponent());
			if (bt) bt->RestartTree();
		}
	}
}

void UEnemyAnim::AnimNotify_OnFalling()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		enemy->GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
		enemy->bFalling = true;
	}
}

void UEnemyAnim::AnimNotify_OnLookTarget()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());
		if (controller)
		{
			enemy->bLookPlayer = true;
			enemy->target = Cast<AActor>(controller->GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::TargetObjKey));
		}
	}
}

void UEnemyAnim::AnimNotify_OnLookTargetEnd()
{
	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	if (enemy)
	{
		AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());
		if (controller)
		{
			enemy->bLookPlayer = false;
			enemy->target = nullptr;
		}
	}
}