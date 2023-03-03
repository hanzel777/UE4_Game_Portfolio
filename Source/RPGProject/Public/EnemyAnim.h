// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_OnAttackCollision();
	UFUNCTION()
	void AnimNotify_OnAttackCollisisonEnd();
	UFUNCTION()
	void AnimNotify_OnSpawnProjectile();
	UFUNCTION()
	void AnimNotify_OnAggroTaskEnd();
	UFUNCTION()
	void AnimNotify_OnAttackTaskEnd();
	UFUNCTION()
	void AnimNotify_OnHitTaskEnd();
	UFUNCTION()
	void AnimNotify_OnFalling();
	UFUNCTION()
	void AnimNotify_OnLookTarget();
	UFUNCTION()
	void AnimNotify_OnLookTargetEnd();

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimationAsset)
	TArray<UAnimationAsset*> aggroAnimAssets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimationAsset)
	TArray<UAnimationAsset*> attackAnimAssets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimationAsset)
	TArray<UAnimationAsset*> hitAnimAssets;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimationAsset)
	TArray<UAnimationAsset*> deathAnimAssets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	TSubclassOf<class AEnemyBaseProjectile> projectile;

public:
	void GetRandomAggroAnimAsset(UAnimationAsset*& animAsset) const;
	void GetRandomAttackAnimAsset(UAnimationAsset*& animAsset) const;
	void GetRandomHitAnimAsset(UAnimationAsset*& animAsset) const;
	void GetRandomDeathAnimAsset(UAnimationAsset*& animAsset) const;
};
