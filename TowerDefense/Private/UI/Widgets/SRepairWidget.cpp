// Fill out your copyright notice in the Description page of Project Settings.

#include "SRepairWidget.h"
#include "SlateOptMacros.h"
#include <SOverlay.h>
#include "../Style/FTowerDefenseStyle.h"
#include "../Style/RepairWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRepairWidget::Construct(const FArguments& InArgs)
{
	RepairStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FRepairStyle>(TEXT("RepairStyle"));

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(FMargin(200.f,200.f,0.f,0.f))
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Top)
			.HeightOverride(100)
			.WidthOverride(100)
			[
				SAssignNew(RepairButton, SButton)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
				.OnClicked(this, &SRepairWidget::OnButtonClicked)
				[
					SAssignNew(RepairImage, SImage)
					.Image(&RepairStyle->RepairIcon)
					.RenderTransformPivot(FVector2D(0.25f,0.25f))
				]
			]
		]
	];

	SetupAnimation();
}

FReply SRepairWidget::OnButtonClicked()
{
	RepairImage->SetImage(&RepairStyle->RepairAction);
	RepairButton->SetEnabled(false);
	return FReply::Handled();
}

void SRepairWidget::SetupAnimation()
{
	RotateSequence = FCurveSequence();
	RotateAnim = RotateSequence.AddCurve(0.f, 1.f, ECurveEaseFunction::CubicInOut);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
