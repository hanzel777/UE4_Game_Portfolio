// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateGameModeBase.h"
#include "CreatePlayerController.h"

ACreateGameModeBase::ACreateGameModeBase()
{
	PlayerControllerClass = ACreatePlayerController::StaticClass();
}