// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class SScoreWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScoreWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)override;

private:
	const struct FNumberSlateStyle* NumberStyle;

	TSharedPtr<SHorizontalBox> ScoreNumsUp;

	TSharedPtr<SHorizontalBox> ScoreNumsDown;

	TArray<SImage*> UpImages;

	TArray<SImage*> DownImages;

	TSharedPtr<SImage> TempImage;
};
