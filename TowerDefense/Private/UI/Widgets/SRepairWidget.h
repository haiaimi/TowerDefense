// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SRepairWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRepairWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply OnButtonClicked();

	void SetupAnimation();

private:
	const struct FRepairStyle* RepairStyle;

	TSharedPtr<SImage> RepairImage;

	TSharedPtr<SButton> RepairButton;

	FCurveHandle RotateAnim;

	FCurveSequence RotateSequence;
};
