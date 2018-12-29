// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMenuGameMode.h"
#include "UI/TDMenuHUD.h"
#include <GameFramework/DefaultPawn.h>


ATDMenuGameMode::ATDMenuGameMode()
{
	HUDClass = ATDMenuHUD::StaticClass();
	DefaultPawnClass = APawn::StaticClass();
}
