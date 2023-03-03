// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ASelectPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ASelectPlayerController();

	TSubclassOf<class USelectUI> selectWidget;
	class USelectUI* selectUI;

	TArray<class ASelectPawn*> selectPawns;

protected:
	virtual void BeginPlay() override;

public:
	void OnSelected(const int32 num);
	void OnStarted();
	void OnDeleted();
};
