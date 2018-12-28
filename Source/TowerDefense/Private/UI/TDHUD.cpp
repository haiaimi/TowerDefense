// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TDHUD.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"
#include "Widgets/SScoreWidget.h"
#include "Widgets/STowerSelectWidget.h"
#include "Widgets/SRepairWidget.h"


ATDHUD::ATDHUD() :
	ScoreWidget(NULL),
	RepairWidget(NULL)
{

}

void ATDHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!ScoreWidget.IsValid() && GEngine)
	{
		SAssignNew(ScoreWidget, SScoreWidget);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(ScoreWidget.ToSharedRef()),
			0
			);

		if (GetWorld())
		{
			if (ATDController* CurController = Cast<ATDController>(GetWorld()->GetFirstPlayerController()))
				ScoreWidget->AddScore(CurController->GetPlayerScore());
		}
	}

	/*if (!RepairWidget.IsValid() && GEngine)
	{
		SAssignNew(RepairWidget, SRepairWidget);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(RepairWidget.ToSharedRef()),
			0
		);
	}*/
}

void ATDHUD::BeginPlay()
{
	Super::BeginPlay();
}
