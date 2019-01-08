// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMenuHUD.h"
#include <Engine/Engine.h>
#include "Widgets/SMainMenuWidget.h"
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"
#include "Delegate.h"
#include <Kismet/GameplayStatics.h>


ATDMenuHUD::ATDMenuHUD():
	MainMenu(NULL)
{
	
}

void ATDMenuHUD::DrawHUD()
{
	if (!MainMenu.IsValid() && GEngine)
	{
		SAssignNew(MainMenu, SMainMenuWidget)
			.Owner(this);

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
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/GameLevels/GameMap"), true);
	}
}

void ATDMenuHUD::QuitGame()
{
	if (GetWorld())
	{
		if (APlayerController* MC = GetWorld()->GetFirstPlayerController())
		{
			MC->ConsoleCommand("quit");
		}
	}
}
