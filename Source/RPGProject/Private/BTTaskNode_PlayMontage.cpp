// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_PlayMontage.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UBTTaskNode_PlayMontage::UBTTaskNode_PlayMontage()
{
	NodeName = TEXT("PlayMontage");
	bCreateNodeInstance = true;

	timerDelegate = FTimerDelegate::CreateUObject(this, &UBTTaskNode_PlayMontage::OnAnimationTimerDone);
}

EBTNodeResult::Type UBTTaskNode_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	myOwnerComp = &OwnerComp;
	timerHandle.Invalidate();

	ACharacter* myCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (myCharacter)
	{
		UAnimInstance* anim = myCharacter->GetMesh()->GetAnimInstance();
		if (anim)
		{
			anim->Montage_Play(montage, montageRate);
			const float FinishDelay = montage->GetMaxCurrentTime();
			OwnerComp.GetAIOwner()->GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, FinishDelay, false);

			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTaskNode_PlayMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* myCharacter = OwnerComp.GetAIOwner()->GetCharacter();
	if (myCharacter)
	{
		UAnimInstance* anim = myCharacter->GetMesh()->GetAnimInstance();
		if (anim) anim->Montage_Stop(0.25f, montage);
	}

	if (timerHandle.IsValid()) OwnerComp.GetAIOwner()->GetWorld()->GetTimerManager().ClearTimer(timerHandle);

	timerHandle.Invalidate();

	return EBTNodeResult::Aborted;
}


void UBTTaskNode_PlayMontage::OnAnimationTimerDone()
{
	if (myOwnerComp) FinishLatentTask(*myOwnerComp, EBTNodeResult::Succeeded);
}