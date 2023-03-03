// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_UpdateWalkSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTaskNode_UpdateWalkSpeed::UBTTaskNode_UpdateWalkSpeed()
{
	NodeName = TEXT("UpdateWalkSpeed");
}

EBTNodeResult::Type UBTTaskNode_UpdateWalkSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	if (character)
	{
		UCharacterMovementComponent* characterMovement = character->GetCharacterMovement();
		characterMovement->MaxWalkSpeed = walkSpeed;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}