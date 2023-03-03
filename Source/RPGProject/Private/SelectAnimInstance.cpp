// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectAnimInstance.h"

USelectAnimInstance::USelectAnimInstance()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Animation/SelectMontage.SelectMontage'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}
}

void USelectAnimInstance::PlaySelectMontage()
{
	Montage_Play(montage);
}