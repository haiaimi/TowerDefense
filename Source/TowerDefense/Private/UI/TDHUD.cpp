﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TDHUD.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"
#include "Widgets/SHUDWidget.h"
#include "Widgets/STowerSelectWidget.h"
#include "Widgets/SRepairWidget.h"
#include "SPauseMenuWidget.h"
#include "TDBomber.h"


ATDHUD::ATDHUD() :
	ScoreWidget(NULL),
	PauseWidget(NULL)
{
}

void ATDHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!ScoreWidget.IsValid() && GEngine)
	{
		SAssignNew(ScoreWidget, SHUDWidget)
			.MyHUD(this);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(ScoreWidget.ToSharedRef()),
			0
			);

		if (GetWorld())
		{
			if (ATDController* CurController = Cast<ATDController>(GetWorld()->GetFirstPlayerController()))
				ScoreWidget->AddScore(CurController->GetPlayerMoney());
		}
	}

	/*if (!PauseWidget.IsValid() && GEngine)
	{
		SAssignNew(PauseWidget, SPauseMenuWidget);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(PauseWidget.ToSharedRef()),
			0
		);
	}*/
}

void ATDHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ATDHUD::SpawnBomber()
{
	//生成空援Actor
	if(GetWorld())
	{
		FActorSpawnParameters SpawnPaprameter;
		SpawnPaprameter.Owner = this;
		GetWorld()->SpawnActor<ATDBomber>(SpawnPaprameter);
	}
}
