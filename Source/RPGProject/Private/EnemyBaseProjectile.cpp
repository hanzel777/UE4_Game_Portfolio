// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseProjectile.h"
#include "RPGPlayer.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEnemyBaseProjectile::AEnemyBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InitialLifeSpan = 8;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(30);
	sphereComp->SetCollisionProfileName("EnemyAttack");

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	particleComp->SetupAttachment(sphereComp);

	projectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile  Movement"));
	projectileComp->SetUpdatedComponent(sphereComp);
	projectileComp->ProjectileGravityScale = 0;
	projectileComp->MaxSpeed = 800;
	projectileComp->InitialSpeed = 800;
	projectileComp->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AEnemyBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemyBaseProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile hit!!! / damage : %.2f"), damage);
		player->OnDamage(damage);

		Destroy();
	}
}

void AEnemyBaseProjectile::SetDamage(const float Damage)
{
	damage = Damage;
}