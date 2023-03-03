// Fill out your copyright notice in the Description page of Project Settings.


#include "FrostGiantAnim.h"
#include "Enemy_Frost_Giant.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "AIController_FrostGiant.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <GameFramework\CharacterMovementComponent.h>

void UFrostGiantAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		speed = FVector::DotProduct(pawn->GetActorForwardVector(), pawn->GetVelocity());
		direction = FVector::DotProduct(pawn->GetActorRightVector(), pawn->GetVelocity());
	}
}

void UFrostGiantAnim::AnimNotify_OnLookTarget()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->bLook = true;
	}
}

void UFrostGiantAnim::AnimNotify_OnLookTargetEnd()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->bLook = false;
	}
}

void UFrostGiantAnim::AnimNotify_OnLeftAttackCollision()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetLeftAttackBoxComp()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UFrostGiantAnim::AnimNotify_OnLeftAttackCollisionEnd()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetLeftAttackBoxComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UFrostGiantAnim::AnimNotify_OnRightAttackCollision()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetRightAttackBoxComp()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UFrostGiantAnim::AnimNotify_OnRightAttackCollisionEnd()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetRightAttackBoxComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UFrostGiantAnim::AnimNotify_OnOverHeadAttackCollision()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetOverHeadAttackComp()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UFrostGiantAnim::AnimNotify_OnOverHeadAttackCollisionEnd()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetOverHeadAttackComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UFrostGiantAnim::AnimNotify_OnJumpAttackCollision()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetJumpAttackComp()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void UFrostGiantAnim::AnimNotify_OnJumpAttackCollisionEnd()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetJumpAttackComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}

void UFrostGiantAnim::AnimNotify_OnJump()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		FVector launchVelocity = pawn->GetActorUpVector() * 2000;
		pawn->LaunchPawn(launchVelocity, false, false);

		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

	}
}

void UFrostGiantAnim::AnimNotify_OnFallowTarget()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AAIController_FrostGiant* ai = Cast<AAIController_FrostGiant>(pawn->GetController());
		if (ai)
		{
			AActor* target = Cast<AActor>(ai->GetBlackboardComponent()->GetValueAsObject(AAIController_FrostGiant::targetObjKey));
			if (target)
			{
				FVector newLocation = target->GetActorLocation();
				newLocation.Z = pawn->GetActorLocation().Z;
				pawn->SetActorLocation(newLocation);

				AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
				if (giant)
				{
					giant->bFollow = true;

					giant->GetMesh()->CastShadow = false;
				}
			}
		}
	}
}

void UFrostGiantAnim::AnimNotify_OnDropDown()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AAIController_FrostGiant* ai = Cast<AAIController_FrostGiant>(pawn->GetController());
		if (ai)
		{	
			FVector dropLocation;
			AActor* target = Cast<AActor>(ai->GetBlackboardComponent()->GetValueAsObject(AAIController_FrostGiant::targetObjKey));
			if (target)
			{
				dropLocation = pawn->GetActorLocation();
				dropLocation.Z = target->GetActorLocation().Z;
				pawn->SetActorLocation(dropLocation);
			}

			AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
			if (giant)
			{
				giant->bFollow = false;

				giant->GetMesh()->CastShadow = true;
			}
		}
	}
}

void UFrostGiantAnim::AnimNotify_OnJumpEnd()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AEnemy_Frost_Giant* giant = Cast<AEnemy_Frost_Giant>(pawn);
		if (giant) giant->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	}
}

void UFrostGiantAnim::AnimNotify_OnAIUnpossess()
{
	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		AAIController* controller = Cast<AAIController>(pawn->GetController());
		if (controller) controller->UnPossess();
	}
}