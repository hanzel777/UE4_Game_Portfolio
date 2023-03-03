// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SelectAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API USelectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USelectAnimInstance();

	void PlaySelectMontage();

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UAnimMontage* montage;
};
