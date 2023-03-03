// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GhostTrail.generated.h"

UCLASS()
class RPGPROJECT_API AGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostTrail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	class UPoseableMeshComponent* poseable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	class UMaterialInstance* ghostMat;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<class UMaterialInstanceDynamic*> dynamicMats;

	UPROPERTY(BlueprintReadOnly , meta = (ExposeOnSpawn = true, AllowPrivateAccess = true))
	USkeletalMeshComponent* mesh;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = true))
	float currentTime;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float delayTime = 1;

	bool isAble;


	void Init();
};
