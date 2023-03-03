// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	static const FName TargetObjKey;
	static const FName bRecognizeKey;
	static const FName bSeePlayerKey;
	static const FName bAggroKey;
	static const FName bAttackKey;
	static const FName bHitKey;
	static const FName bDeathKey;
	static const FName bNotCalculateDistanceKey;

	UFUNCTION()
	void OnTargetPerception(AActor* Actor, FAIStimulus Stimulus);

	float sightRadius = 1300;
	float loseSightRadius = 2000;
	float fielOfView = 60;

	virtual FGenericTeamId GetGenericTeamId() const override { return teamID; }

protected:
	virtual void OnPossess(APawn* InPawn) override;


	FGenericTeamId teamID;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UBlackboardData* BBAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAISenseConfig_Sight* sightConfig;
};
