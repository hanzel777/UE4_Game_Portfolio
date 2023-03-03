// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SelectPawn.generated.h"

UCLASS()
class RPGPROJECT_API ASelectPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int32 pawnNum = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* widgetComp;

public:
	// Sets default values for this pawn's properties
	ASelectPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 GetPawnNum() const;


	//�÷��̾� ������ ������ ���̺��� �޾ƿ� �ִϸ��̼ǰ� �Ž� ����
	void SetMeshAndAnim(USkeletalMesh* mesh, UClass* anim, FString nickName, FString className, int32 level);
	//���� ���� ��, �ִϸ��̼� ����
	void OnSelected();

	void SetSlotColor();
};
