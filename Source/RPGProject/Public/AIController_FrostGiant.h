// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_FrostGiant.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AAIController_FrostGiant : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_FrostGiant();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UBehaviorTree* BTAsset;
	class UBlackboardData* BBAsset;

public:
	static const FName targetObjKey;
};
