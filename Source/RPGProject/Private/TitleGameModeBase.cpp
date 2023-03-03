// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameModeBase.h"
#include "TitlePlayerController.h"

ATitleGameModeBase::ATitleGameModeBase()
{
	PlayerControllerClass = ATitlePlayerController::StaticClass();
}