// Fill out your copyright notice in the Description page of Project Settings.

#include "SRepairWidget.h"
#include "SlateOptMacros.h"
#include <SOverlay.h>
#include "../Style/FTowerDefenseStyle.h"
#include "../Style/RepairWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRepairWidget::Construct(const FArguments& InArgs)
{
	TowerBase = InArgs._TowerBase;
	bInRepair = false;
	RepairStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FRepairStyle>(TEXT("RepairStyle"));
	
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(FMargin(InArgs._SpawnPos.X,InArgs._SpawnPos.Y,0.f,0.f))
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
				.RenderTransformPivot(FVector2D(0.25f,0.75f))
				[
					SAssignNew(RepairImage, SImage)
					.Image(&RepairStyle->RepairIcon)
					.RenderTransformPivot(FVector2D(0.5f,0.5f))
					.RenderTransform(FSlateRenderTransform(FQuat2D(-0.5f*PI)))
				]
			]
		]
	];
}

void SRepairWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!bInRepair)return;
	const float CurLerp = RotateAnim.GetLerp();
	const float RotateAngle = CurLerp * 90.f - 45.f;
	RepairButton->SetRenderTransform(FSlateRenderTransform(FQuat2D(FMath::DegreesToRadians(RotateAngle))));
	if (RotateSequence.IsAtEnd())
		RotateSequence.Reverse();
	if (RotateSequence.IsInReverse()&&RotateSequence.IsAtStart())
		RotateSequence.Play(this->AsShared());
}

FReply SRepairWidget::OnButtonClicked()
{
	if (!bInRepair)
	{
		RepairImage->SetImage(&RepairStyle->RepairAction);
		SetupAnimation(); //开始修理的动画
	}
	if (TowerBase)
	{
		TowerBase->PayForRepair();
		TowerBase->HealSelf();
	}
	bInRepair = true;
	RepairButton->SetEnabled(false);
	
	return FReply::Handled();
}

void SRepairWidget::SetupAnimation()
{
	RotateSequence = FCurveSequence();
	RotateAnim = RotateSequence.AddCurve(0.f, 1.f, ECurveEaseFunction::CubicInOut);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
