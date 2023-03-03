// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassData.h"
#include "WizardClassData.generated.h"

/**
 * 
 */
class RPGPROJECT_API WizardClassData
{
public:
	WizardClassData();
	~WizardClassData();
};

USTRUCT(Atomic, BlueprintType)
struct FWizardClassData : public FClassData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWizardClassType wizardClassType = EWizardClassType::Sorceress;

	FWizardClassData()
	{
		classType = EClassType::Wizard;
	}
};