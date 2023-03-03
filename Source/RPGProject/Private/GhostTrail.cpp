// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostTrail.h"
#include <Components\PoseableMeshComponent.h>
#include <Kismet\KismetMaterialLibrary.h>

// Sets default values
AGhostTrail::AGhostTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	poseable = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMeshComponent"));
	SetRootComponent(poseable);

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempMat(TEXT("MaterialInstanceConstant'/Game/Materials/MT_GhostTrail_Instance.MT_GhostTrail_Instance'"));
	if (tempMat.Succeeded())
	{
		ghostMat = tempMat.Object;
	}
}

// Called when the game starts or when spawned
void AGhostTrail::BeginPlay()
{
	Super::BeginPlay();
	
	Init();
}

// Called every frame
void AGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isAble)
	{
		currentTime += DeltaTime;
		for (int i = 0; i < dynamicMats.Num(); i++)
		{
			dynamicMats[i]->SetScalarParameterValue(TEXT("Opacity"), (delayTime - currentTime) / delayTime);
		}

		if (currentTime >= delayTime)
		{
			Destroy();
		}
	}
}

void AGhostTrail::Init()
{
	isAble = false;
	currentTime = 0;

	if (mesh)
	{
		poseable->SetSkeletalMesh(mesh->SkeletalMesh);
		poseable->CopyPoseFromSkeletalComponent(mesh);

		for (int i = 0; i < poseable->GetMaterials().Num(); i++)
		{
			dynamicMats.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ghostMat));
			poseable->SetMaterial(i, dynamicMats[i]);
		}

		isAble = true;
	}
}