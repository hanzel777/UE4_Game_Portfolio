// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_EnemyAnimation.h"
#include "EnemyBaseCharacter.h"
#include "EnemyAnim.h"
#include "AIController.h"
#include "Animation/AnimationAsset.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_EnemyAnimation::UBTTaskNode_EnemyAnimation()
{
	NodeName = TEXT("EnemyAnimation");
}

EBTNodeResult::Type UBTTaskNode_EnemyAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if(bFirstTransitionBool)OwnerComp.GetBlackboardComponent()->SetValueAsBool(key.SelectedKeyName, false);

	UAnimationAsset* animAsset = nullptr;
	UAnimMontage* montage = nullptr;
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	if (character)
	{
		CachedSkelMesh = character->GetMesh();
		if (CachedSkelMesh)
		{
			UEnemyAnim* anim = Cast<UEnemyAnim>(CachedSkelMesh->GetAnimInstance());
			if (anim)
			{
				switch (animType)
				{
				case EAnimationType::Aggro:
					anim->GetRandomAggroAnimAsset(animAsset);
					break;
				case EAnimationType::Attack:
					anim->GetRandomAttackAnimAsset(animAsset);
					break;
				case EAnimationType::Hit:
					anim->GetRandomHitAnimAsset(animAsset);
					break;
				case EAnimationType::Death:
					anim->GetRandomDeathAnimAsset(animAsset);
					montage = Cast<UAnimMontage>(animAsset);
					if (montage)
					{
						anim->Montage_Play(montage);
						return EBTNodeResult::InProgress;
					}
					return EBTNodeResult::Failed;
				}

				if (animAsset)
				{
					montage = Cast<UAnimMontage>(animAsset);
					if (montage)
					{
						anim->StopAllMontages(0.25f);
						anim->Montage_Play(montage);
						return EBTNodeResult::InProgress;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTaskNode_EnemyAnimation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(key.SelectedKeyName))OwnerComp.GetBlackboardComponent()->SetValueAsBool(key.SelectedKeyName, false);

	return EBTNodeResult::Aborted;
}