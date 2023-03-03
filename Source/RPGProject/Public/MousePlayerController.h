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

	//플레이어 일반 공격 시, 회전 속도
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
		float rotationRate = 0.6;

	bool GetbMouseLeftClick() const;
	//공격 중인지 확인
	bool GetAttack() const;
	//공격 설정
	void SetAttack(const bool isAttack);
	//공격 끝으로 변경
	void AttackEnd();
	//조작 가능 여부 확인
	bool GetControl() const;
	//조작 불가능으로 변경
	void OutOfControl();
	//러쉬 여부 확인
	bool GetRush() const;
	//러쉬 종료
	void RushEnd();
	void SetHit(const bool hit) { bHit = hit; }
	void SetStiff(const bool stiff) { bStiff = stiff; }

	void SetAnim();

	//AIBlueprintHelperLibrary를 이용해 location으로 이동
	void SetNewDestination(FVector destination);

protected:
	//공격 중인지 확인
	bool bAttack = false;
	// 러쉬 여부 확인
	bool bRush = false;
	// 피격 여부 확인
	bool bHit = false;
	// 경직 여부 확인
	bool bStiff = false;

	//자신의 폰
	class APawn* pawn;
	//자신의 애님
	class UPlayerAnim* anim;

	//플레이어 받아오기
	virtual void BeginPlay() override;
	//마우스 입력 맵핑
	virtual void SetupInputComponent() override;

	//로컬 플레이어 프레임 틱
	virtual void PlayerTick(float DeltaTime) override;

private:
	//마우스 우클릭 여부 확인
	bool bMouseRightClick = false;
	//마우스 좌클릭 여부 확인
	bool bMouseLeftClick = false;
	//마우스 좌클릭 released 타이머 제어 변수
	bool bAlreadyReleased = false;
	//조작 가능 여부 확인
	bool bControl = true;

	// 공격 기능
	void Attack();
	// 마우스 커서 보기
	void LookMouseCursur();

	//마우스 우클릭을 눌렀을 때, 마우스 클릭 여부 트루
	void InputMouseRightPressed();
	//마우스 우클릭을 땟을 때, 마우스 클릭 여부 거짓
	void InputMouseRightReleased();

	//마우스 좌클릭을 눌렀을 때, 마우스 클릭 여부 트루
	void InputMouseLeftPressed();
	//마우스 좌클릭을 땟을 때, 마우스 클릭 여부 거짓
	void InputMouseLeftReleased();

	//마우스 커서가 누른 지점을 받아와 SetNewDestination의 매개변수로 호출
	void MoveToMouseClickLocation();
};
