// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CreatePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROJECT_API ACreatePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACreatePlayerController();

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<class UCreateUI> createWidget;

	class UCreateUI* createUI;

	class UDataTable* classTable;
};
