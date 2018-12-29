// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget)
	{}
	SLATE_ARGUMENT(FSimpleDelegate, OnPressed)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)override;

private:
	void SetupAnimation();

	void ShowRank();

	void BackToMenu();

private:
	TSharedPtr<SVerticalBox> RankContainer;

	TSharedPtr<SButton> BackButton;

	TArray<FCurveHandle> RankAnims;

	FCurveSequence RankSequence;
};
