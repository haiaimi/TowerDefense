// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class STowerSelectWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STowerSelectWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetupAnimation();

private:
	const struct FNumberSlateStyle* NumberStyle;

	const struct FTowerSelectStyle* TowerSelectStyle;
	
	TSharedPtr<SBox> WidgetBox;
	TSharedPtr<SHorizontalBox> TowerCostNumbers;

	FCurveHandle MoveAnim;

	FCurveSequence MoveSecquence;

	/**控件在屏幕上移动的方向*/
	FVector2D MoveDir;

	FVector2D StartPos;
};
