// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnim();

	void Init();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//���콺 ��Ŭ�� �޺� ����
	UFUNCTION()
		void AnimNotify_OnComboAttack();
	//���콺 ��Ŭ�� ���� ����
	UFUNCTION()
		void AnimNotify_OnAttackEnd();
	//��ų ���� ����
	UFUNCTION()
		void AnimNotify_OnSkillEnd();
	//���� ����
	UFUNCTION()
		void AnimNotify_OnRushEnd();
	//���� ����
	UFUNCTION()
		void AnimNotify_OnStiffEnd();
	//����Ʈ�� Ʈ��
	UFUNCTION()
		void AnimNotify_OnPerfectZone();
	//����Ʈ�� false
	UFUNCTION()
		void AnimNotify_OnPerfectZoneEnd();

	UFUNCTION()
		void AnimNotify_OnGameOverUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnChargeReleased();

	//���콺 ��Ŭ�� ����
	void OnAttack();
	//��ų ����
	void OnSkill(FName skillName);
	//����
	void OnRush();
	//����Ŀ ����
	void OnBerserk();
	//����Ŀ ���� ����
	UFUNCTION(BlueprintImplementableEvent)
	void OnBerserkEnd();

	void OnDie();

	void OnStiff();
	void OnHit();

	FName currentSkillName;

protected:
	//�÷��̾� �̵� ���ǵ�
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		float speed;

private:
	//��Ŭ�� �޺� ���� ī����
	int32 comboCnt = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool isPerfectZone = false;

	//�÷��̾� ��
	class APawn* pawn;

	//��ġ ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* punchMontage;
	//���� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* attackMontage;
	//��ų ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* skillMontage;
	//���� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* rushMontage;
	//�̵� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* moveMontage;
	//����Ŀ ���� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* berserkMontage;
	//���� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* dieMontage;
	//���� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* stiffMontage;
	//�ǰ� ��Ÿ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* hitMontage;

	void ClearImmunity();
};
