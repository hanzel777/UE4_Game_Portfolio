// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "RPGPlayer.h"
#include "PlayerAnim.h"
#include "SkillWidget.h"
#include <GameFramework\CharacterMovementComponent.h>
#include <Kismet\KismetMathLibrary.h>
#include <Components\CapsuleComponent.h>
#include <Kismet\GameplayStatics.h>

void ARPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//스킬 바인드
	InputComponent->BindAction(TEXT("Skill0"), IE_Pressed, this, &ARPGPlayerController::OnSkill_0);
	InputComponent->BindAction(TEXT("Skill1"), IE_Pressed, this, &ARPGPlayerController::OnSkill_1);
	InputComponent->BindAction(TEXT("Skill2"), IE_Pressed, this, &ARPGPlayerController::OnSkill_2);
	InputComponent->BindAction(TEXT("Skill3"), IE_Pressed, this, &ARPGPlayerController::OnSkill_3);
	InputComponent->BindAction(TEXT("Skill4"), IE_Pressed, this, &ARPGPlayerController::OnSkill_4);
	InputComponent->BindAction(TEXT("Skill5"), IE_Pressed, this, &ARPGPlayerController::OnSkill_5);
	InputComponent->BindAction(TEXT("Skill6"), IE_Pressed, this, &ARPGPlayerController::OnSkill_6);
	InputComponent->BindAction(TEXT("Skill7"), IE_Pressed, this, &ARPGPlayerController::OnSkill_7);

	InputComponent->BindAction(TEXT("Skill0"), IE_Released, this, &ARPGPlayerController::ReSkill_0);
	InputComponent->BindAction(TEXT("Skill1"), IE_Released, this, &ARPGPlayerController::ReSkill_1);
	InputComponent->BindAction(TEXT("Skill2"), IE_Released, this, &ARPGPlayerController::ReSkill_2);
	InputComponent->BindAction(TEXT("Skill3"), IE_Released, this, &ARPGPlayerController::ReSkill_3);
	InputComponent->BindAction(TEXT("Skill4"), IE_Released, this, &ARPGPlayerController::ReSkill_4);
	InputComponent->BindAction(TEXT("Skill5"), IE_Released, this, &ARPGPlayerController::ReSkill_5);
	InputComponent->BindAction(TEXT("Skill6"), IE_Released, this, &ARPGPlayerController::ReSkill_6);
	InputComponent->BindAction(TEXT("Skill7"), IE_Released, this, &ARPGPlayerController::ReSkill_7);

	//러쉬 바인드
	InputComponent->BindAction(TEXT("Rush"), IE_Pressed, this, &ARPGPlayerController::OnRush);

	//퀵슬롯 바인드
	InputComponent->BindAction(TEXT("Item0"), IE_Pressed, this, &ARPGPlayerController::OnItem_0);
	InputComponent->BindAction(TEXT("Item1"), IE_Pressed, this, &ARPGPlayerController::OnItem_1);
	InputComponent->BindAction(TEXT("Item2"), IE_Pressed, this, &ARPGPlayerController::OnItem_2);
	InputComponent->BindAction(TEXT("Item3"), IE_Pressed, this, &ARPGPlayerController::OnItem_3);
	InputComponent->BindAction(TEXT("Item4"), IE_Pressed, this, &ARPGPlayerController::OnItem_4);
	InputComponent->BindAction(TEXT("Item5"), IE_Pressed, this, &ARPGPlayerController::OnItem_5);
	InputComponent->BindAction(TEXT("Item6"), IE_Pressed, this, &ARPGPlayerController::OnItem_6);
	InputComponent->BindAction(TEXT("Item7"), IE_Pressed, this, &ARPGPlayerController::OnItem_7);
}

void ARPGPlayerController::OnSKill(FSkillData* skillData)
{
	ARPGPlayer* rpgPlayer = Cast<ARPGPlayer>(pawn);
	if (rpgPlayer->GetCurrentMp() < skillData->spendMp || bStiff || bHit || !GetControl() || bAttack || bRush || !skillData->bSkillReady || skillData->skillName == "" || !Cast<ARPGPlayer>(pawn)->IsWeaponeEquip()) return;
	bAttack = true;

	bStiffImmunity = skillData->bStiffImmunity;
	bHitImmunity = skillData->bHitImmunity;

	anim->OnSkill(*FString::Printf(TEXT("%s"), *skillData->skillName));


	pawn->GetMovementComponent()->StopActiveMovement();

	skillData->bSkillReady = false;
	rpgPlayer->SetCurrentMp(rpgPlayer->GetCurrentMp() - skillData->spendMp);
}

void ARPGPlayerController::ReSkill(FSkillData* skillData)
{
	if (skillData->skillType == ESkillType::Charging && anim->currentSkillName == FName(skillData->skillName))
	{
		anim->OnChargeReleased();
	}
}

void ARPGPlayerController::OnItem(FItemData* itemData)
{
	if (itemData->bReady && itemData->itemName != "" && itemData->itemCount > 0 && GetControl())
	{
		useItemDelegate.ExecuteIfBound(itemData->itemName);
		itemData->bReady = false;
	}
}

void ARPGPlayerController::OnRush()
{
	if (!bRush && bRushReady && GetControl() && !bHit)
	{
		bRushReady = false;
		bStiffImmunity = true;
		bHitImmunity = true;

		//공격 중이라면 어택을 취소하고 러쉬
		bAttack = false;
		//이동 중이라면 이동을 멈춤
		pawn->GetMovementComponent()->StopActiveMovement();

		bRush = true;
		anim->OnRush();
	}
}

void ARPGPlayerController::OnSkill_0() { OnSKill(skillDatas[0]); }
void ARPGPlayerController::OnSkill_1() { OnSKill(skillDatas[1]); }
void ARPGPlayerController::OnSkill_2() { OnSKill(skillDatas[2]); }
void ARPGPlayerController::OnSkill_3() { OnSKill(skillDatas[3]); }
void ARPGPlayerController::OnSkill_4() { OnSKill(skillDatas[4]); }
void ARPGPlayerController::OnSkill_5() { OnSKill(skillDatas[5]); }
void ARPGPlayerController::OnSkill_6() { OnSKill(skillDatas[6]); }
void ARPGPlayerController::OnSkill_7() { OnSKill(skillDatas[7]); }

void ARPGPlayerController::ReSkill_0() { ReSkill(skillDatas[0]); }
void ARPGPlayerController::ReSkill_1() { ReSkill(skillDatas[1]); }
void ARPGPlayerController::ReSkill_2() { ReSkill(skillDatas[2]); }
void ARPGPlayerController::ReSkill_3() { ReSkill(skillDatas[3]); }
void ARPGPlayerController::ReSkill_4() { ReSkill(skillDatas[4]); }
void ARPGPlayerController::ReSkill_5() { ReSkill(skillDatas[5]); }
void ARPGPlayerController::ReSkill_6() { ReSkill(skillDatas[6]); }
void ARPGPlayerController::ReSkill_7() { ReSkill(skillDatas[7]); }

void ARPGPlayerController::OnItem_0() { OnItem(itemDatas[0]); }
void ARPGPlayerController::OnItem_1() { OnItem(itemDatas[1]); }
void ARPGPlayerController::OnItem_2() { OnItem(itemDatas[2]); }
void ARPGPlayerController::OnItem_3() { OnItem(itemDatas[3]); }
void ARPGPlayerController::OnItem_4() { OnItem(itemDatas[4]); }
void ARPGPlayerController::OnItem_5() { OnItem(itemDatas[5]); }
void ARPGPlayerController::OnItem_6() { OnItem(itemDatas[6]); }
void ARPGPlayerController::OnItem_7() { OnItem(itemDatas[7]); }


void ARPGPlayerController::CollTime(float DeltaTime)
{
	if (!bRushReady)
	{
		rushCurrentTime += DeltaTime;
		if (rushCurrentTime >= rushDelayTime)
		{
			rushCurrentTime = 0;
			bRushReady = true;
		}
	}

	for (auto skillData : skillDatas)
	{
		if (!skillData->bSkillReady)
		{
			skillData->currentTime += DeltaTime;
			if (skillData->currentTime >= skillData->coolTime)
			{
				skillData->currentTime = 0;
				skillData->bSkillReady = true;
			}
		}
	}

	for (auto itemData : itemDatas)
	{
		if (!itemData->bReady)
		{
			itemData->currentTime += DeltaTime;
			if (itemData->currentTime >= itemData->delayTime)
			{
				itemData->currentTime = 0;
				itemData->bReady = true;
			}
		}
	}
}

void ARPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CollTime(DeltaTime);
}

void ARPGPlayerController::OnStiff(AActor* OverlappedActor, const float speed)
{
	if (bStiffImmunity || bHitImmunity || !GetControl()) return;

	bStiff = true;

	//공격 중이라면 어택을 취소
	bAttack = false;
	//이동 중이라면 이동을 멈춤
	pawn->GetMovementComponent()->StopActiveMovement();

	ARPGPlayer* rpgPlayer = Cast<ARPGPlayer>(pawn);
	if (rpgPlayer)
	{
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(rpgPlayer->GetActorLocation(), OverlappedActor->GetActorLocation());
		lookRotation.Roll = 0;
		lookRotation.Pitch = 0;
		rpgPlayer->SetActorRotation(lookRotation);

		FVector launchVelocity = rpgPlayer->GetActorForwardVector() * -speed;
		rpgPlayer->LaunchCharacter(launchVelocity, true, true);
	}

	anim->OnStiff();
}

void ARPGPlayerController::OnHit(AActor* OverlappedActor, const float speed, const float upSpeed)
{
	if (bHitImmunity || bHit || !GetControl()) return;

	bHit = true;

	//공격 중이라면 어택을 취소
	bAttack = false;
	//이동 중이라면 이동을 멈춤
	pawn->GetMovementComponent()->StopActiveMovement();

	ARPGPlayer* rpgPlayer = Cast<ARPGPlayer>(pawn);
	if (rpgPlayer)
	{
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(rpgPlayer->GetActorLocation(), OverlappedActor->GetActorLocation());
		lookRotation.Roll = 0;
		lookRotation.Pitch = 0;
		rpgPlayer->SetActorRotation(lookRotation);

		FVector launchVelocity = rpgPlayer->GetActorForwardVector() * -speed + rpgPlayer->GetActorUpVector() * upSpeed;
		rpgPlayer->LaunchCharacter(launchVelocity, true, true);
	}

	anim->OnHit();

	FTimerHandle hitHandle;
	GetWorld()->GetTimerManager().SetTimer(hitHandle, FTimerDelegate::CreateLambda([this]()->void {anim->StopAllMontages(0.3f); bHit = false; }), 2, false);
}