// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	TSubclassOf<class UUserWidget> titleWidget;
	class UUserWidget* titleUI;

public:
	ATitlePlayerController();

	virtual void BeginPlay() override;
};
