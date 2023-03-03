// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FrostGiantAnim.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UFrostGiantAnim : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float direction;

private:
	UFUNCTION()
	void AnimNotify_OnLookTarget();
	UFUNCTION()
	void AnimNotify_OnLookTargetEnd();

	UFUNCTION()
	void AnimNotify_OnLeftAttackCollision();
	UFUNCTION()
	void AnimNotify_OnLeftAttackCollisionEnd();
	UFUNCTION()
	void AnimNotify_OnRightAttackCollision();
	UFUNCTION()
	void AnimNotify_OnRightAttackCollisionEnd();
	UFUNCTION()
	void AnimNotify_OnOverHeadAttackCollision();
	UFUNCTION()
	void AnimNotify_OnOverHeadAttackCollisionEnd();
	UFUNCTION()
	void AnimNotify_OnJumpAttackCollision();
	UFUNCTION()
	void AnimNotify_OnJumpAttackCollisionEnd();

	//캐릭터를 LaunchCharacter를 통해 공중으로 이동 플레이와 충돌 할 수 없게 무시처리
	UFUNCTION()
	void AnimNotify_OnJump();
	//캐릭터가 타겟을 공중에서 쫓아가게 만듬
	UFUNCTION()
	void AnimNotify_OnFallowTarget();
	//캐릭터가 타겟의 위치로 설정
	UFUNCTION()
	void AnimNotify_OnDropDown();
	//캐릭터의 콜라이더를 다시 플레이와 충돌로 변경
	UFUNCTION()
	void AnimNotify_OnJumpEnd();

	UFUNCTION()
	void AnimNotify_OnAIUnpossess();
};
