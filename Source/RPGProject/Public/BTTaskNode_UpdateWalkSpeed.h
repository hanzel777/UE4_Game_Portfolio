// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_UpdateWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UBTTaskNode_UpdateWalkSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_UpdateWalkSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnyWhere, Category = Movement)
	float walkSpeed;
};
