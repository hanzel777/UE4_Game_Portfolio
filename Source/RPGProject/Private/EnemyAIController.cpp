// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "RPGPlayer.h"
#include "EnemyBaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

const FName AEnemyAIController::TargetObjKey = TEXT("TargetActor");
const FName AEnemyAIController::bRecognizeKey = TEXT("bRecognizePlayer");
const FName AEnemyAIController::bSeePlayerKey = TEXT("bSeePlayer");
const FName AEnemyAIController::bAggroKey = TEXT("bAggro");
const FName AEnemyAIController::bAttackKey = TEXT("bAttack");
const FName AEnemyAIController::bHitKey = TEXT("bHit");
const FName AEnemyAIController::bDeathKey = TEXT("bDeath");
const FName AEnemyAIController::bNotCalculateDistanceKey = TEXT("bNotCalculateDistance");

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> tempBT(TEXT("BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (tempBT.Succeeded())
	{
		BTAsset = tempBT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> tempBB(TEXT("BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'"));
	if (tempBB.Succeeded())
	{
		BBAsset = tempBB.Object;
	}


	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent")));
	sightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	sightConfig->SightRadius = sightRadius;
	sightConfig->LoseSightRadius = loseSightRadius;
	sightConfig->PeripheralVisionAngleDegrees = fielOfView;
	

	GetPerceptionComponent()->SetDominantSense(sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerception);
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}

	AEnemyBaseCharacter* enemy = Cast<AEnemyBaseCharacter>(InPawn);
	if (enemy)
	{
		teamID = enemy->teamID;
	}
}

void AEnemyAIController::OnTargetPerception(AActor* Actor, FAIStimulus Stimulus)
{
	ARPGPlayer* player = Cast<ARPGPlayer>(Actor);
	if (player == nullptr)return;

	if (Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsBool(bRecognizeKey, true);
		Blackboard->SetValueAsBool(bSeePlayerKey, true);

		if (Blackboard->GetValueAsObject(TargetObjKey) == nullptr)
		{
			Blackboard->SetValueAsObject(TargetObjKey, Actor);
			Blackboard->SetValueAsBool(bAggroKey, true);
		}
	}
	else if(!Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsBool(bSeePlayerKey, false);
	}
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* pawn = Cast<APawn>(&Other);
	if(pawn == nullptr) return ETeamAttitude::Neutral;

	const ARPGPlayer* player = Cast<ARPGPlayer>(&Other);
	const IGenericTeamAgentInterface* AI_TI = Cast<IGenericTeamAgentInterface>(pawn);
	if (player == nullptr && AI_TI == nullptr) return ETeamAttitude::Neutral;

	if (player != nullptr) return ETeamAttitude::Hostile;

	FGenericTeamId otherID = NULL;
	if (AI_TI != nullptr)otherID = AI_TI->GetGenericTeamId();


	if (otherID == GetGenericTeamId())
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Neutral;
	}
}