// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy_Frost_Giant.generated.h"

UCLASS()
class RPGPROJECT_API AEnemy_Frost_Giant : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_Frost_Giant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void LookTarget();

	void FollowTarget();

	UFUNCTION()
	void OnAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverHeadAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnJumpAttackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = status, meta = (AllowPrivateAccess = true))
	float currentHp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = status, meta = (AllowPrivateAccess = true))
	float maxHp = 5000;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = status, meta = (AllowPrivateAccess = true))
	float attackDamage = 100;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, category = status, meta = (AllowPrivateAccess = true))
	float jumpFallowSpeed = 400;

	class AAIController* ai;
	AActor* target;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UBoxComponent* leftAttackBoxComp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UBoxComponent* rightAttackBoxComp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class USphereComponent* overHeadAttackComp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class USphereComponent* jumpAttackComp;

public:
	float GetCurrentHp() const { return currentHp; }
	float GetMaxHp() const { return maxHp; }
	class UBoxComponent* GetLeftAttackBoxComp() const { return leftAttackBoxComp; }
	class UBoxComponent* GetRightAttackBoxComp() const { return rightAttackBoxComp; }
	class USphereComponent* GetOverHeadAttackComp() const { return overHeadAttackComp; }
	class USphereComponent* GetJumpAttackComp() const { return jumpAttackComp; }

	UFUNCTION(BlueprintCallable)
	void OnDamage(const float damage);

	bool bLook = false;
	bool bFollow = false;
};
