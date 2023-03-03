// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ConvertBoolenValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_ConvertBoolenValue::UBTTaskNode_ConvertBoolenValue()
{
	NodeName = TEXT("Convert Boolen Value");

	boolenKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskNode_ConvertBoolenValue, boolenKey));
}

EBTNodeResult::Type UBTTaskNode_ConvertBoolenValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool bOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsBool(boolenKey.SelectedKeyName);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(boolenKey.SelectedKeyName, !bOrigin);
	return EBTNodeResult::Succeeded;
}