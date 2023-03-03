// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseCharacter.h"
#include "EnemyAIController.h"
#include "EnemyHealthBar.h"
#include "RPGPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include <Components\CapsuleComponent.h>
#include <Components\BoxComponent.h>
#include <GameFramework\CharacterMovementComponent.h>
#include <Kismet\KismetMathLibrary.h>

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//자연스럽게 돌게 하기 위해 컨트롤러로테이션을 false
	//charactermovement에 orient rotation을 true
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(GetRootComponent());
	widgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	widgetComp->SetRelativeLocation(FVector(0, 0, 90));
	widgetComp->SetDrawAtDesiredSize(true);

	ConstructorHelpers::FClassFinder<UUserWidget> tempHealthBar(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_EnemyHealthBar.WBP_EnemyHealthBar_C'"));
	if (tempHealthBar.Succeeded())
	{
		healthBar = tempHealthBar.Class;
	}

	widgetComp->SetWidgetClass(healthBar);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Collision"));
	boxComp->SetupAttachment(GetRootComponent());
	boxComp->SetRelativeLocation(FVector(70, 0, -30));
	boxComp->SetCollisionProfileName(TEXT("NoCollision"));
	boxComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Owner;
}

// Called when the game starts or when spawned
void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	currentHp = maxHp;
	widgetComp->SetVisibility(false);

	RangeFix();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBaseCharacter::OnAttackCollisionOverlap);

	OnBeginCursorOver.AddDynamic(this, &AEnemyBaseCharacter::OnCursorBeginOver);
	OnEndCursorOver.AddDynamic(this, &AEnemyBaseCharacter::OnCursorEndOver);

	originPos = GetActorLocation();
}

void AEnemyBaseCharacter::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player)
	{
		player->OnDamage(attackDamage);
	}
}

void AEnemyBaseCharacter::OnCursorBeginOver(AActor* TouchedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCursorBeginOver"));
	UEnemyHealthBar* hpBar = Cast<UEnemyHealthBar>(widgetComp->GetWidget());
	if (hpBar)
	{
		if (!widgetComp->IsVisible())
		{
			widgetComp->SetVisibility(true);
			hpBar->UpdateHpPercent(currentHp / maxHp);
		}

		hpBar->MonsterNameVisibility(true, monsterName);
	}
}

void AEnemyBaseCharacter::OnCursorEndOver(AActor* TouchedActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCursorEndOver"));
	UEnemyHealthBar* hpBar = Cast<UEnemyHealthBar>(widgetComp->GetWidget());
	if (hpBar)
	{
		hpBar->MonsterNameVisibility(false, monsterName);
	}

	if (widgetVisibleHandle.IsValid() == false)
	{
		widgetComp->SetVisibility(false);
	}
}

// Called every frame
void AEnemyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FallingDown(DeltaTime);

	LookTarget();
}

void AEnemyBaseCharacter::LookTarget()
{
	if (bLookPlayer && target)
	{
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
		lookRotation.Roll = 0;
		lookRotation.Pitch = 0;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), lookRotation, GetWorld()->DeltaTimeSeconds, 500));
	}
}

// Called to bind functionality to input
void AEnemyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBaseCharacter::RangeFix()
{
	if (chaseRange <= recognizeRange)
	{
		chaseRange = recognizeRange + 500;
	}
}

void AEnemyBaseCharacter::OnDamage(const float damage, UObject* attakedObj)
{
	if (currentHp <= 0) return;

	currentHp -= damage;

	AEnemyAIController* enemyController = Cast<AEnemyAIController>(GetController());
	if (enemyController)
	{
		if (currentHp <= 0)
		{
			currentHp = 0;
			isDead = true;
			enemyController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bDeathKey, true);
		}
		else if(Cast<ARPGPlayer>(attakedObj))
		{
			bRecognize = true;
			enemyController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bRecognizeKey, true);
			enemyController->GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::TargetObjKey, attakedObj);
			enemyController->GetBlackboardComponent()->SetValueAsBool(AEnemyAIController::bHitKey, true);
		}

		UpdateWidgetCompVisible();
	}
}

void AEnemyBaseCharacter::FallingDown(const float DeltaTime)
{
	if (isDead)
	{
		if (!bFalling)
		{
			enemyDeadLocation = GetActorLocation();
		}

		else
		{
			FVector newLocation;

			newLocation = GetActorLocation() + FVector::DownVector * fallingSpeed * DeltaTime;

			if (FVector::Distance(newLocation, enemyDeadLocation) > 200) Destroy();

			SetActorLocation(newLocation);
		}
	}
}


void AEnemyBaseCharacter::UpdateWidgetCompVisible()
{
	if (widgetComp->IsVisible())
	{
		if (widgetVisibleHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(widgetVisibleHandle);
	}
	else
	{
		widgetComp->SetVisibility(true);
	}

	UEnemyHealthBar* hpBar = Cast<UEnemyHealthBar>(widgetComp->GetWidget());
	if (hpBar)
	{
		hpBar->UpdateHpPercent(currentHp / maxHp);
	}
	widgetVisibleHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(widgetVisibleHandle, FTimerDelegate::CreateLambda([this]()->void {widgetComp->SetVisibility(false); widgetVisibleHandle.Invalidate(); }), hpViewTime, false);
}