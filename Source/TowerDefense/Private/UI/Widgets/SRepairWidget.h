// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "TDTowerBase.h"

/**
 * 
 */
class SRepairWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRepairWidget):
		_TowerBase(nullptr)
	{}
	SLATE_ARGUMENT(FVector2D, SpawnPos)
	SLATE_ARGUMENT(ATDTowerBase*, TowerBase)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)override;

	FReply OnButtonClicked();

	void SetupAnimation();

private:
	bool bInRepair; 

	const struct FRepairStyle* RepairStyle;

	ATDTowerBase* TowerBase;

	TSharedPtr<SImage> RepairImage;

	TSharedPtr<SButton> RepairButton;

	FCurveHandle RotateAnim;

	FCurveSequence RotateSequence;
};
