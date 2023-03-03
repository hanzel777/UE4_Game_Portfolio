// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Frost_Giant.h"
#include "Enemy_Frost_Giant.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Frost_Giant::UBTService_Frost_Giant()
{
	NodeName = TEXT("FrostGiant_Service");

	hpPercentKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_Frost_Giant, hpPercentKey));
	targetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_Frost_Giant, targetKey), AActor::StaticClass());
	distanceFromTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_Frost_Giant, distanceFromTargetKey));
}

void UBTService_Frost_Giant::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	myOwnerComp = &OwnerComp;

	frostGiant = Cast<AEnemy_Frost_Giant>(OwnerComp.GetAIOwner()->GetPawn());
	if (frostGiant == nullptr)return;

	CalculateHpPercent();
	CalculateDistanceFromTarget();
}

void UBTService_Frost_Giant::CalculateHpPercent()
{
	float hpPercent = frostGiant->GetCurrentHp() / frostGiant->GetMaxHp();

	myOwnerComp->GetBlackboardComponent()->SetValueAsFloat(hpPercentKey.SelectedKeyName, hpPercent);
}

void UBTService_Frost_Giant::CalculateDistanceFromTarget()
{
	UObject* targetObj = myOwnerComp->GetBlackboardComponent()->GetValueAsObject(targetKey.SelectedKeyName);
	if (targetObj)
	{
		AActor* targetActor = Cast<AActor>(targetObj);
		float distance = frostGiant->GetDistanceTo(targetActor);
		myOwnerComp->GetBlackboardComponent()->SetValueAsFloat(distanceFromTargetKey.SelectedKeyName, distance);
	}
}