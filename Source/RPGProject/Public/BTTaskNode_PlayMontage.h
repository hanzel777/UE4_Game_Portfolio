// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TimerManager.h"
#include "BTTaskNode_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTTaskNode_PlayMontage : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTTaskNode_PlayMontage();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(Category = Montage, EditAnywhere)
	class UAnimMontage* montage;

	UPROPERTY(Category = Montage, EditAnywhere)
	float montageRate = 1;

	UPROPERTY()
	class UBehaviorTreeComponent* myOwnerComp;

	FTimerDelegate timerDelegate;
	FTimerHandle timerHandle;

	void OnAnimationTimerDone();
};
