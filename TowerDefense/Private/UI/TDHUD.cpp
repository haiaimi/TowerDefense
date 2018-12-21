// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TDHUD.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"
#include "Widgets/SScoreWidget.h"
#include "Widgets/STowerSelectWidget.h"


ATDHUD::ATDHUD() :
	ScoreWidget(NULL),
	TowerSelectWidget(NULL)
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

		//ScoreWidget->SetVisibility(EVisibility::HitTestInvisible);
	}

	/*if (!TowerSelectWidget.IsValid() && GEngine)
	{
		SAssignNew(TowerSelectWidget, STowerSelectWidget);

		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SWeakWidget)
			.PossiblyNullContent(TowerSelectWidget.ToSharedRef()),
			0
		);
	}*/
}

void ATDHUD::BeginPlay()
{
	Super::BeginPlay();
}
