// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MousePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API AMousePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMousePlayerController();

	//�÷��̾� �Ϲ� ���� ��, ȸ�� �ӵ�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
		float rotationRate = 0.6;

	bool GetbMouseLeftClick() const;
	//���� ������ Ȯ��
	bool GetAttack() const;
	//���� ����
	void SetAttack(const bool isAttack);
	//���� ������ ����
	void AttackEnd();
	//���� ���� ���� Ȯ��
	bool GetControl() const;
	//���� �Ұ������� ����
	void OutOfControl();
	//���� ���� Ȯ��
	bool GetRush() const;
	//���� ����
	void RushEnd();
	void SetHit(const bool hit) { bHit = hit; }
	void SetStiff(const bool stiff) { bStiff = stiff; }

	void SetAnim();

	//AIBlueprintHelperLibrary�� �̿��� location���� �̵�
	void SetNewDestination(FVector destination);

protected:
	//���� ������ Ȯ��
	bool bAttack = false;
	// ���� ���� Ȯ��
	bool bRush = false;
	// �ǰ� ���� Ȯ��
	bool bHit = false;
	// ���� ���� Ȯ��
	bool bStiff = false;

	//�ڽ��� ��
	class APawn* pawn;
	//�ڽ��� �ִ�
	class UPlayerAnim* anim;

	//�÷��̾� �޾ƿ���
	virtual void BeginPlay() override;
	//���콺 �Է� ����
	virtual void SetupInputComponent() override;

	//���� �÷��̾� ������ ƽ
	virtual void PlayerTick(float DeltaTime) override;

private:
	//���콺 ��Ŭ�� ���� Ȯ��
	bool bMouseRightClick = false;
	//���콺 ��Ŭ�� ���� Ȯ��
	bool bMouseLeftClick = false;
	//���콺 ��Ŭ�� released Ÿ�̸� ���� ����
	bool bAlreadyReleased = false;
	//���� ���� ���� Ȯ��
	bool bControl = true;

	// ���� ���
	void Attack();
	// ���콺 Ŀ�� ����
	void LookMouseCursur();

	//���콺 ��Ŭ���� ������ ��, ���콺 Ŭ�� ���� Ʈ��
	void InputMouseRightPressed();
	//���콺 ��Ŭ���� ���� ��, ���콺 Ŭ�� ���� ����
	void InputMouseRightReleased();

	//���콺 ��Ŭ���� ������ ��, ���콺 Ŭ�� ���� Ʈ��
	void InputMouseLeftPressed();
	//���콺 ��Ŭ���� ���� ��, ���콺 Ŭ�� ���� ����
	void InputMouseLeftReleased();

	//���콺 Ŀ���� ���� ������ �޾ƿ� SetNewDestination�� �Ű������� ȣ��
	void MoveToMouseClickLocation();
};
