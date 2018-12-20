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

	void SetupAnimation();

	FORCEINLINE int32 GetSpecifiedNumber(int32 Index)
	{
		int32 Res = (DestScore / (FMath::Pow(10, 4 - Index)));
		return Res % 10;
	}

	void AddScore(int32 AddedScore);

private:
	/**面板当前显示的分数*/
	int32 CurSocre;

	/**目标分数*/
	int32 DestScore;

	const struct FNumberSlateStyle* NumberStyle;

	TSharedPtr<SHorizontalBox> ScoreNumsUp;

	TSharedPtr<SHorizontalBox> ScoreNumsDown;

	FCurveSequence AnimSequence;

	FCurveHandle ScrollAnim;

	TArray<int32> UpNumbers;

	TArray<int32> DownNumbers;
};
