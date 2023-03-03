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

	//ĳ���͸� LaunchCharacter�� ���� �������� �̵� �÷��̿� �浹 �� �� ���� ����ó��
	UFUNCTION()
	void AnimNotify_OnJump();
	//ĳ���Ͱ� Ÿ���� ���߿��� �Ѿư��� ����
	UFUNCTION()
	void AnimNotify_OnFallowTarget();
	//ĳ���Ͱ� Ÿ���� ��ġ�� ����
	UFUNCTION()
	void AnimNotify_OnDropDown();
	//ĳ������ �ݶ��̴��� �ٽ� �÷��̿� �浹�� ����
	UFUNCTION()
	void AnimNotify_OnJumpEnd();

	UFUNCTION()
	void AnimNotify_OnAIUnpossess();
};
