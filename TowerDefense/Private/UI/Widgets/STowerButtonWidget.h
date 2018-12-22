// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class STowerButtonWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STowerButtonWidget)
	{}
	SLATE_ATTRIBUTE(TOptional<FSlateRenderTransform>, ButtonScale)
	SLATE_EVENT(FOnClicked, OnClicked)
	SLATE_ARGUMENT(int32, TowerCost)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FORCEINLINE int32 GetSpecifiedNumber(int32 DestNum, int32 Index)
	{
		int32 Res = (DestNum / (FMath::Pow(10, 3 - Index)));
		return Res % 10;
	}

private:
	TSharedPtr<SHorizontalBox> TowerCostNumbers;

	const struct FNumberSlateStyle* NumberStyle;

	const struct FTowerSelectStyle* TowerSelectStyle;
};
