// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMenuHUD.h"
#include <Engine/Engine.h>
#include "Widgets/SMainMenuWidget.h"
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"
#include "Delegate.h"


ATDMenuHUD::ATDMenuHUD():
	MainMenu(NULL)
{

}

void ATDMenuHUD::DrawHUD()
{
	if (!MainMenu.IsValid() && GEngine)
	{
		FSimpleDelegate OnPressed, OnQuit;
		OnPressed.BindUObject(this, &ATDMenuHUD::LaunchGame);
		OnQuit.BindUObject(this, &ATDMenuHUD::QuitGame);
		SAssignNew(MainMenu, SMainMenuWidget)
			.OnStart(OnPressed)
			.OnQuit(OnQuit);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(MainMenu.ToSharedRef()),
			0
			);
	}

	Super::DrawHUD();
}

void ATDMenuHUD::LaunchGame()
{
	if (GetWorld())
		GetWorld()->ServerTravel(TEXT("/Game/Levels/GameMap"), false, true);
}

void ATDMenuHUD::QuitGame()
{
	if (GetWorld())
	{
		if (APlayerController* MC = GetWorld()->GetFirstPlayerController())
			MC->ConsoleCommand("quit");
	}
}
