// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBaseCharacter.generated.h"

UCLASS()
class RPGPROJECT_API AEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();

	UPROPERTY(EditAnyWhere, Category = Range)
	float attackRange;
	UPROPERTY(EditAnyWhere, Category = Range)
	float recognizeRange;
	UPROPERTY(EditAnyWhere, Category = Range)
	float chaseRange = 2000;

	UPROPERTY(EditAnywhere, Category = Patrol)
	FVector originPos;
	UPROPERTY(EditAnywhere, Category = Patrol)
	float patrolRadius;

	UPROPERTY(EditDefaultsOnly, Category = Status)
	FString monsterName;
	UPROPERTY(EditDefaultsOnly, Category = Status)
	int32 teamID = 0;
	UPROPERTY(VisibleAnywhere, Category = Status)
	bool isDead;
	UPROPERTY(VisibleAnywhere, Category = Status)
	bool bFalling;
	UPROPERTY(VisibleAnywhere, Category = Status)
	bool bLookPlayer;
	UPROPERTY(VisibleAnywhere, Category = Status)
	AActor* target;
	UPROPERTY(VisibleAnywhere, Category = Status)
	float fallingSpeed = 50;

	UPROPERTY(VisibleAnywhere, Category = BlackboardService)
	bool bDeath;
	UPROPERTY(VisibleAnywhere, Category = BlackboardService)
	bool bHit;
	UPROPERTY(VisibleAnywhere, Category = BlackboardService)
	bool bAttack;
	UPROPERTY(VisibleAnywhere, Category = BlackboardService)
	bool bRecognize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void OnDamage(const float damage, UObject* attakedObj);

	class UBoxComponent* GetBoxComponent() const { return boxComp; }

	float GetAttackDamage() const { return attackDamage; }

	TSubclassOf<class UUserWidget> healthBar;

private:
	UPROPERTY(EditAnywhere, Category = Status, meta = (AllowPriavteAccess = true))
	float maxHp = 500;
	UPROPERTY(VisibleAnywhere, Category = Status, meta = (AllowPriavteAccess = true))
	float currentHp;
	UPROPERTY(EditAnywhere, Category = Status, meta = (AllowPriavteAccess = true))
	float attackDamage = 10;
	UPROPERTY(EditAnywhere, Category = Status, meta = (AllowPriavteAccess = true))
	FVector enemyDeadLocation;

	UPROPERTY(EditAnywhere, meta = (AllowPriavteAccess = true))
	class UWidgetComponent* widgetComp;
	UPROPERTY(EditAnywhere, Category = HealthBar, meta = (AllowPriavteAccess = true))
	float hpViewTime = 3;

	UPROPERTY(EditAnywhere, meta = (AllowPriavteAccess = true))
	class UBoxComponent* boxComp;

	UFUNCTION()
	void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCursorBeginOver(AActor* TouchedActor);
	UFUNCTION()
	void OnCursorEndOver(AActor* TouchedActor);

	FTimerHandle widgetVisibleHandle;

	//추적 범위가 인식 범위보다 작은 문제 등을 수정
	void RangeFix();

	void FallingDown(const float DeltaTime);

	void UpdateWidgetCompVisible();

	void LookTarget();
};
