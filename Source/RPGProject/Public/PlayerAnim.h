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

	//마우스 좌클릭 콤보 공격
	UFUNCTION()
		void AnimNotify_OnComboAttack();
	//마우스 좌클릭 공격 종료
	UFUNCTION()
		void AnimNotify_OnAttackEnd();
	//스킬 공격 종료
	UFUNCTION()
		void AnimNotify_OnSkillEnd();
	//러쉬 종료
	UFUNCTION()
		void AnimNotify_OnRushEnd();
	//경직 종료
	UFUNCTION()
		void AnimNotify_OnStiffEnd();
	//퍼팩트존 트루
	UFUNCTION()
		void AnimNotify_OnPerfectZone();
	//퍼팩트존 false
	UFUNCTION()
		void AnimNotify_OnPerfectZoneEnd();

	UFUNCTION()
		void AnimNotify_OnGameOverUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnChargeReleased();

	//마우스 좌클릭 공격
	void OnAttack();
	//스킬 공격
	void OnSkill(FName skillName);
	//러쉬
	void OnRush();
	//버서커 폭주
	void OnBerserk();
	//버서커 폭주 종료
	UFUNCTION(BlueprintImplementableEvent)
	void OnBerserkEnd();

	void OnDie();

	void OnStiff();
	void OnHit();

	FName currentSkillName;

protected:
	//플레이어 이동 스피드
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
		float speed;

private:
	//좌클릭 콤보 공격 카운터
	int32 comboCnt = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool isPerfectZone = false;

	//플레이어 폰
	class APawn* pawn;

	//펀치 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* punchMontage;
	//어택 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* attackMontage;
	//스킬 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* skillMontage;
	//러쉬 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* rushMontage;
	//이동 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* moveMontage;
	//버서커 폭주 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* berserkMontage;
	//죽음 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* dieMontage;
	//경직 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* stiffMontage;
	//피격 몽타주
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* hitMontage;

	void ClearImmunity();
};
