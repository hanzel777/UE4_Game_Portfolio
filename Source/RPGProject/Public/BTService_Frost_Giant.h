// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Frost_Giant.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTService_Frost_Giant : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Frost_Giant();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector hpPercentKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector targetKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector distanceFromTargetKey;

private:
	void CalculateHpPercent();
	void CalculateDistanceFromTarget();

	UBehaviorTreeComponent* myOwnerComp;
	class AEnemy_Frost_Giant* frostGiant;
};
