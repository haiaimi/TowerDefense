// Fill out your copyright notice in the Description page of Project Settings.

#include "TDGameMode.h"
#include "TDController.h"


ATDGameMode::ATDGameMode()
{
	PlayerControllerClass = ATDController::StaticClass();
}

void ATDGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
