// Fill out your copyright notice in the Description page of Project Settings.

#include "TDGameMode.h"
#include "TDController.h"
#include "UI/TDHUD.h"


ATDGameMode::ATDGameMode()
{
	PlayerControllerClass = ATDController::StaticClass();
	HUDClass = ATDHUD::StaticClass();
}

void ATDGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
