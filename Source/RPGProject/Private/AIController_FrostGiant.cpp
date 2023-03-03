// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_FrostGiant.h"

const FName AAIController_FrostGiant::targetObjKey = TEXT("TargetActor");

AAIController_FrostGiant::AAIController_FrostGiant()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> tempBT(TEXT("BehaviorTree'/Game/AI/BT_Forge_King.BT_Forge_King'"));
	if (tempBT.Succeeded())
	{
		BTAsset = tempBT.Object;
	}

	ConstructorHelpers::FObjectFinder<UBlackboardData> tempBB(TEXT("BlackboardData'/Game/AI/BB_Forge_King.BB_Forge_King'"));
	if (tempBB.Succeeded())
	{
		BBAsset = tempBB.Object;
	}
}

void AAIController_FrostGiant::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}
}