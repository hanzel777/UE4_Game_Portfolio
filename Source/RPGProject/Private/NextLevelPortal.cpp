// Fill out your copyright notice in the Description page of Project Settings.


#include "NextLevelPortal.h"
#include "RPGPlayer.h"
#include "RPGGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Level.h"

// Sets default values
ANextLevelPortal::ANextLevelPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	SetRootComponent(capsuleComp);
	capsuleComp->SetCollisionProfileName(TEXT("NoCollision"));
	capsuleComp->SetMobility(EComponentMobility::Static);
	capsuleComp->SetCapsuleHalfHeight(50);
	capsuleComp->SetCapsuleRadius(50);

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Portal Particle"));
	particleComp->SetupAttachment(capsuleComp);

	ConstructorHelpers::FObjectFinder<UParticleSystem> ps(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Summon/P_Summon_Portal.P_Summon_Portal'"));
	if (ps.Succeeded())
	{
		particleComp->SetTemplate(ps.Object);
		particleComp->SetRelativeLocation(FVector(0, 0, -120));
	}
}

// Called when the game starts or when spawned
void ANextLevelPortal::BeginPlay()
{
	Super::BeginPlay();

	capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ANextLevelPortal::OnNextLevel);

	particleComp->SetVisibility(false);
}

// Called every frame
void ANextLevelPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANextLevelPortal::OnNextLevel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player == nullptr) return;

	URPGGameInstance* rpgGameInstance = Cast<URPGGameInstance>(UGameplayStatics::GetGameInstance(player->GetController()));
	if (rpgGameInstance)
	{
		rpgGameInstance->UpdatedCuurentPlayerDataInUserTable();
		rpgGameInstance->OpenNextLevel();
	}
}