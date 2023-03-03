// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_EnemyAnimation.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAnimationType : uint8
{
	Aggro,
	Attack,
	Hit,
	Death
};

UCLASS()
class RPGPROJECT_API UBTTaskNode_EnemyAnimation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_EnemyAnimation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnyWhere, Category = Animation)
	EAnimationType animType;
	UPROPERTY(EditAnyWhere, Category = Animation)
	float addDelay = 0;

	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector key;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	struct FBlackboardKeySelector bNotCalculateKey;
	UPROPERTY(EditAnyWhere, Category = Blackboard)
	bool bFirstTransitionBool;

	UPROPERTY()
	USkeletalMeshComponent* CachedSkelMesh;
};
