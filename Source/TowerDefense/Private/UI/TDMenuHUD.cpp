// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMenuHUD.h"
#include <Engine/Engine.h>
#include "Widgets/SMainMenuWidget.h"
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"


ATDMenuHUD::ATDMenuHUD():
	MainMenu(NULL)
{

}

void ATDMenuHUD::DrawHUD()
{
	if (!MainMenu.IsValid() && GEngine)
	{
		SAssignNew(MainMenu, SMainMenuWidget);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(MainMenu.ToSharedRef()),
			0
			);
	}

	Super::DrawHUD();
}
