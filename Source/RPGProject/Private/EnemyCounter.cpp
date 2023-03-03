// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCounter.h"
#include "EnemyBaseCharacter.h"
#include "Item.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCounter::AEnemyCounter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCounter::BeginPlay()
{
	Super::BeginPlay();
	
	itemBox = Cast<AItem>(UGameplayStatics::GetActorOfClass(GetWorld(), AItem::StaticClass()));
}

// Called every frame
void AEnemyCounter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBaseCharacter::StaticClass(), enemys);

	if (enemys.Num() == 0)
	{
		if (itemBox)
		{
			itemBox->GetSphereComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			itemBox->GetMeshComponent()->SetVisibility(true);
		}
		Destroy();
	}
}

