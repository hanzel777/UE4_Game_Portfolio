// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "RPGPlayerController.h"
#include "RPGPlayer.h"
#include "GameOverUI.h"
#include <Kismet\KismetMathLibrary.h>
#include <GameFramework\CharacterMovementComponent.h>
#include <Components\CapsuleComponent.h>

UPlayerAnim::UPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempAttackMontage(TEXT("AnimMontage'/Game/Animation/Warrior/Berserker/AMT_Attack.AMT_Attack'"));
	if (tempAttackMontage.Succeeded())
	{
		attackMontage = tempAttackMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempSkillMontage(TEXT("AnimMontage'/Game/Animation/Warrior/Berserker/AMT_Skill.AMT_Skill'"));
	if (tempSkillMontage.Succeeded())
	{
		skillMontage = tempSkillMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempRushMontage(TEXT("AnimMontage'/Game/Animation/Warrior/Berserker/AMT_Rush.AMT_Rush'"));
	if (tempRushMontage.Succeeded())
	{
		rushMontage = tempRushMontage.Object;
	}
}

void UPlayerAnim::Init()
{
	pawn = TryGetPawnOwner();

	if (pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerAnim Panw Succes"));
		AMousePlayerController* mouseController = Cast<AMousePlayerController>(pawn->GetController());
		if (mouseController)
		{
			UE_LOG(LogTemp, Warning, TEXT("UPlayerAnim mouseController Succes"));

			mouseController->SetAnim();
		}
	}
}


void UPlayerAnim::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);

	if (pawn)
	{
		ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
		if (player)
		{
			speed = player->GetVelocity().Size();
		}
	}
}

void UPlayerAnim::AnimNotify_OnComboAttack()
{
	if (pawn)
	{
		AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
		if (controller && controller->GetAttack() && controller->GetbMouseLeftClick())
		{
			comboCnt++;
			ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
			if (player)
			{
				if (player->IsWeaponeEquip())
				{
					Montage_JumpToSection(*FString::Printf(TEXT("Combo%d"), comboCnt), attackMontage);
				}
				else if (!player->IsWeaponeEquip() && punchMontage)
				{
					Montage_JumpToSection(*FString::Printf(TEXT("Combo%d"), comboCnt), punchMontage);
				}
			}
		}
	}
}

void UPlayerAnim::AnimNotify_OnAttackEnd()
{
	if (pawn)
	{
		AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
		if (controller)
		{
			UE_LOG(LogTemp, Warning, TEXT("AnimNotify_OnAttackEnd: %d"), comboCnt);

			comboCnt = 0;
			controller->AttackEnd();
			ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
			if (player)
			{
				if (player->IsWeaponeEquip())
				{
					Montage_Stop(0.25f, attackMontage);
				}
				else if (!player->IsWeaponeEquip() && punchMontage)
				{
					Montage_Stop(0.1f, punchMontage);
				}
			}
		}
	}
}

void UPlayerAnim::OnAttack()
{
	AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
	ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
	if (controller && player)
	{
		FHitResult hit;
		controller->GetHitResultUnderCursor(ECC_Visibility, true, hit);
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(pawn->GetActorLocation(), hit.ImpactPoint);
		player->SetActorRotation(lookRotation);
		player->SetActorRotation(FRotator(0, pawn->GetActorRotation().Yaw, 0));

		if (player->IsWeaponeEquip())
		{
			Montage_Play(attackMontage, player->GetAttackSpeed());
		}
		else if(!player->IsWeaponeEquip() && punchMontage)
		{
			Montage_Play(punchMontage, player->GetAttackSpeed());
		}
		else
		{
			controller->AttackEnd();
		}
	}
}

void UPlayerAnim::AnimNotify_OnSkillEnd()
{
	isPerfectZone = false;
	currentSkillName = FName(TEXT(""));
	
	ClearImmunity();

	if (pawn)
	{
		AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
		ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
		if (controller && player)
		{
			controller->AttackEnd();
			Montage_Stop(0.25f, skillMontage);
			player->SetActorRotation(FRotator(0, pawn->GetActorRotation().Yaw, 0));
		}
	}
}

void UPlayerAnim::OnSkill(FName skillName)
{
	currentSkillName = skillName;
	isPerfectZone = false;

	AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
	ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
	if (controller && player)
	{
		FHitResult hit;
		controller->GetHitResultUnderCursor(ECC_Visibility, true, hit);
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(pawn->GetActorLocation(), hit.ImpactPoint);
		player->SetActorRotation(lookRotation);
		player->SetActorRotation(FRotator(0, pawn->GetActorRotation().Yaw, 0));

		Montage_Play(skillMontage, player->GetAttackSpeed());
		Montage_JumpToSection(skillName, skillMontage);
	}
}

void UPlayerAnim::OnRush()
{
	if (pawn)
	{
		AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
		ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
		if (controller && player)
		{
			Montage_Play(rushMontage);

			player->GetCharacterMovement()->MaxAcceleration = 10000;
			player->GetCharacterMovement()->MaxWalkSpeed = player->GetJumpSpeed();

			FHitResult hit;
			controller->GetHitResultUnderCursor(ECC_Visibility, true, hit);
			FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(pawn->GetActorLocation(), hit.ImpactPoint);
			player->SetActorRotation(lookRotation);
			player->SetActorRotation(FRotator(0, pawn->GetActorRotation().Yaw, 0));

			FVector destination = (pawn->GetActorForwardVector() * player->GetJumpDistance()) + pawn->GetActorLocation();
			controller->SetNewDestination(destination);
		}
	}
}

void UPlayerAnim::OnBerserk()
{
	AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
	if (controller)
	{
		pawn->GetMovementComponent()->StopActiveMovement();
		controller->SetAttack(true);
		Montage_Play(berserkMontage);
	}
}

void UPlayerAnim::AnimNotify_OnRushEnd()
{
	ClearImmunity();

	if (pawn)
	{
		pawn->GetMovementComponent()->StopActiveMovement();
		AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
		ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
		if (player)
		{
			player->GetCharacterMovement()->MaxAcceleration = 2048;
			player->GetCharacterMovement()->MaxWalkSpeed = player->GetRunSpeed();
		}
		if (controller)
		{
			controller->RushEnd();
			Montage_Stop(0.25f, rushMontage);
		}
	}
}

void UPlayerAnim::AnimNotify_OnPerfectZone()
{
	isPerfectZone = true;
}

void UPlayerAnim::AnimNotify_OnPerfectZoneEnd()
{
	isPerfectZone = false;
}

void UPlayerAnim::OnChargeReleased_Implementation()
{
	if (isPerfectZone)
	{
		Montage_JumpToSection(*FString::Printf(TEXT("%sPerfectZone"), *currentSkillName.ToString()), skillMontage);
	}
	else
	{
		AnimNotify_OnSkillEnd();
	}
}

void UPlayerAnim::OnDie()
{
	AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
	if (controller)
	{
		controller->StopMovement();
		controller->OutOfControl();
	}
	Montage_Play(dieMontage);
}

void UPlayerAnim::AnimNotify_OnGameOverUI()
{
	ARPGPlayer* player = Cast<ARPGPlayer>(pawn);
	if (player)
	{
		player->GetGameOverUI()->AddToViewport();
	}
}

void UPlayerAnim::OnStiff()
{
	StopAllMontages(0.1f);
	Montage_Play(stiffMontage);
}

void UPlayerAnim::AnimNotify_OnStiffEnd()
{
	if (pawn)
	{
		AMousePlayerController* controller = Cast<AMousePlayerController>(pawn->GetController());
		if (controller)
		{
			controller->SetStiff(false);
		}
	}
}

void UPlayerAnim::OnHit()
{
	StopAllMontages(0.1f);
	Montage_Play(hitMontage);
}

void UPlayerAnim::ClearImmunity()
{
	if (pawn)
	{
		ARPGPlayerController* controller = Cast<ARPGPlayerController>(pawn->GetController());
		if (controller)
		{
			controller->bStiffImmunity = false;
			controller->bHitImmunity = false;
		}
	}
}