// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/TDHUD.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include "Common/HAIAIMIHelper.h"
#include "Widgets/SScoreWidget.h"


ATDHUD::ATDHUD() :
	ScoreWidget(NULL)
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
}

void ATDHUD::BeginPlay()
{
	Super::BeginPlay();
}
