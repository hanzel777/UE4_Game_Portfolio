// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Frost_Giant.h"
#include "AIController_FrostGiant.h"
#include "RPGPlayer.h"
#include "RPGPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AEnemy_Frost_Giant::AEnemy_Frost_Giant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(190);
	GetCapsuleComponent()->SetCapsuleRadius(100);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Boss"));

	leftAttackBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Attack Collision"));
	leftAttackBoxComp->SetupAttachment(GetMesh(), TEXT("b_MF_Weapon_L"));
	leftAttackBoxComp->SetBoxExtent(FVector(100, 50, 50));
	leftAttackBoxComp->SetCollisionProfileName(TEXT("EnemyAttack"));
	leftAttackBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	rightAttackBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Attack Collision"));
	rightAttackBoxComp->SetupAttachment(GetMesh(), TEXT("b_MF_Weapon_R"));
	rightAttackBoxComp->SetBoxExtent(FVector(100, 50, 50));
	rightAttackBoxComp->SetCollisionProfileName(TEXT("EnemyAttack"));
	rightAttackBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	overHeadAttackComp = CreateDefaultSubobject<USphereComponent>(TEXT("Over Head Attack Collision"));
	overHeadAttackComp->SetupAttachment(GetRootComponent());
	overHeadAttackComp->SetRelativeLocation(FVector(220, 30, -170));
	overHeadAttackComp->SetSphereRadius(300);
	overHeadAttackComp->SetCollisionProfileName(TEXT("EnemyAttack"));
	overHeadAttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	jumpAttackComp = CreateDefaultSubobject<USphereComponent>(TEXT("Jump Attack Collision"));
	jumpAttackComp->SetupAttachment(GetRootComponent());
	jumpAttackComp->SetRelativeLocation(FVector(0, 0, -170));
	jumpAttackComp->SetSphereRadius(500);
	jumpAttackComp->SetCollisionProfileName(TEXT("EnemyAttack"));
	jumpAttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/InfinityBladeAdversaries/Enemy/Enemy_Frost_Giant/SK_Enemy_FrostGiant_Captain.SK_Enemy_FrostGiant_Captain'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -190), FRotator(0, -90, 0));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempABP(TEXT("AnimBlueprint'/Game/InfinityBladeAdversaries/Enemy/Enemy_Frost_Giant/Enemy_Frost_Giant_All_Animation/ABP_Frost_Giant.ABP_Frost_Giant_C'"));
	if (tempABP.Succeeded())
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(tempABP.Class);
	}

	AIControllerClass = AAIController_FrostGiant::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AEnemy_Frost_Giant::BeginPlay()
{
	Super::BeginPlay();
	
	currentHp = maxHp;

	ai = Cast<AAIController>(GetController());

	leftAttackBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Frost_Giant::OnAttackBeginOverlap);
	rightAttackBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Frost_Giant::OnAttackBeginOverlap);
	overHeadAttackComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Frost_Giant::OnOverHeadAttackBeginOverlap);
	jumpAttackComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Frost_Giant::OnJumpAttackBeginOverlap);
}

void AEnemy_Frost_Giant::OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player)
	{
		player->OnDamage(attackDamage);
		ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(player->GetController());
		if (rpgController)
		{
			rpgController->OnStiff(this, 100);
		}
	}
}

void AEnemy_Frost_Giant::OnOverHeadAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player)
	{
		player->OnDamage(attackDamage * 1.5f);
		ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(player->GetController());
		if (rpgController)
		{
			rpgController->OnHit(this, 500, 250);
		}
	}
}

void AEnemy_Frost_Giant::OnJumpAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player)
	{
		player->OnDamage(attackDamage * 2);
		ARPGPlayerController* rpgController = Cast<ARPGPlayerController>(player->GetController());
		if (rpgController)
		{
			rpgController->OnHit(this, 1000, 500);
		}
	}
}

// Called every frame
void AEnemy_Frost_Giant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookTarget();

	FollowTarget();
}

void AEnemy_Frost_Giant::LookTarget()
{
	if (bLook)
	{
		if (target == nullptr) target = Cast<AActor>(ai->GetBlackboardComponent()->GetValueAsObject(AAIController_FrostGiant::targetObjKey));
		if (target == nullptr)return;

		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target->GetActorLocation());
		lookRotation.Roll = 0;
		lookRotation.Pitch = 0;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), lookRotation, GetWorld()->DeltaTimeSeconds, 10));
	}
}

void AEnemy_Frost_Giant::FollowTarget()
{
	if (bFollow)
	{
		if (target == nullptr) target = Cast<AActor>(ai->GetBlackboardComponent()->GetValueAsObject(AAIController_FrostGiant::targetObjKey));
		if (target == nullptr)return;

		FVector targetLocation = target->GetActorLocation();
		targetLocation.Z = GetActorLocation().Z;

		FVector direction = (targetLocation - GetActorLocation()).GetSafeNormal();

		FVector newLocation = GetActorLocation() + direction * jumpFallowSpeed * GetWorld()->DeltaTimeSeconds;
		newLocation.Z = 1000;
		SetActorLocation(newLocation);
	}
}

// Called to bind functionality to input
void AEnemy_Frost_Giant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy_Frost_Giant::OnDamage(const float damage)
{
	if (currentHp <= 0) return;

	currentHp -= damage;

	if (currentHp <= 0)
	{
		currentHp = 0;
		return;
	}
}