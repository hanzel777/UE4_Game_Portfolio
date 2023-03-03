// Fill out your copyright notice in the Description page of Project Settings.


#include "MousePlayerController.h"
#include "PlayerAnim.h"
#include "RPGGameInstance.h"
#include <Blueprint\AIBlueprintHelperLibrary.h>
#include <GameFramework\CharacterMovementComponent.h>
#include <GameFramework\Character.h>
#include <Kismet\KismetMathLibrary.h>
#include <Kismet\GameplayStatics.h>

AMousePlayerController::AMousePlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
}

void AMousePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);


	SetInputMode(inputMode);

	pawn = GetPawn();
}

void AMousePlayerController::SetAnim()
{
	if (pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMousePlayerController pawn succes"));


		ACharacter* character = Cast<ACharacter>(pawn);
		if (character)
		{
			UE_LOG(LogTemp, Warning, TEXT("AMousePlayerController character succes"));

			anim = Cast<UPlayerAnim>(character->GetMesh()->GetAnimInstance());
			if(anim)
			UE_LOG(LogTemp, Warning, TEXT("AMousePlayerController anim succes"));

		}

	}
}

void AMousePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("MouseMove"), IE_Pressed, this, &AMousePlayerController::InputMouseRightPressed);
	InputComponent->BindAction(TEXT("MouseMove"), IE_Released, this, &AMousePlayerController::InputMouseRightReleased);

	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMousePlayerController::InputMouseLeftPressed);
	InputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AMousePlayerController::InputMouseLeftReleased);
}

void AMousePlayerController::InputMouseRightPressed()
{
	bMouseRightClick = true;
}

void AMousePlayerController::InputMouseRightReleased()
{
	bMouseRightClick = false;
}

void AMousePlayerController::InputMouseLeftPressed()
{
	bMouseLeftClick = true;
}

void AMousePlayerController::InputMouseLeftReleased()
{
	if (bAlreadyReleased) return;
	FTimerHandle mouseLeftHandle;
	GetWorld()->GetTimerManager().SetTimer(mouseLeftHandle, FTimerDelegate::CreateLambda([this]()->void { bMouseLeftClick = false; bAlreadyReleased = false; }), 0.2f, false);
	bAlreadyReleased = true;
}

void AMousePlayerController::SetNewDestination(FVector destination)
{
	if (pawn)
	{
		if (120.0f < FVector::Dist(destination, pawn->GetActorLocation()))
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, destination);
	}
}

void AMousePlayerController::MoveToMouseClickLocation()
{
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, true, hit);
	SetNewDestination(hit.ImpactPoint);
}

void AMousePlayerController::Attack()
{
	if (anim)
	{
		pawn->GetMovementComponent()->StopActiveMovement();
		if (!bAttack)
		{
			anim->OnAttack();
		}

		bAttack = true;
	}
	LookMouseCursur();
}

void AMousePlayerController::LookMouseCursur()
{
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, true, hit);

	if (pawn)
	{
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(pawn->GetActorLocation(), hit.ImpactPoint);
		pawn->SetActorRotation(FMath::RInterpTo(pawn->GetActorRotation(), lookRotation, GetWorld()->DeltaTimeSeconds, rotationRate));
		pawn->SetActorRotation(FRotator(0, pawn->GetActorRotation().Yaw, 0));
	}
}

bool AMousePlayerController::GetbMouseLeftClick() const
{
	return bMouseLeftClick;
}

bool AMousePlayerController::GetAttack() const
{
	return bAttack;
}

void AMousePlayerController::SetAttack(const bool isAttack)
{
	bAttack = isAttack;
}

void AMousePlayerController::AttackEnd()
{
	bAttack = false;
}

bool AMousePlayerController::GetControl() const
{
	return bControl;
}

void AMousePlayerController::OutOfControl()
{
	bControl = false;
}

bool AMousePlayerController::GetRush() const
{
	return bRush;
}

void AMousePlayerController::RushEnd()
{
	bRush = false;
}

void AMousePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bControl && !bHit && !bStiff)
	{
		if (bMouseRightClick && !bRush && !bAttack)
		{
			MoveToMouseClickLocation();
		}
		if (bMouseLeftClick && !bRush)
		{
			Attack();
		}
	}
}