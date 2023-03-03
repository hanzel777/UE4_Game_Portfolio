// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_EnemySelectorService.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTService_EnemySelectorService : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_EnemySelectorService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector tagetObjKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector targetPosKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bDeathKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bHitKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bAttackKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bRecognizeKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bSeePlayerKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bAggroKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bNotCalculateKey;

	void CaculateDistanceAndUpdateKeyValue(UBehaviorTreeComponent& OwnerComp);
};
