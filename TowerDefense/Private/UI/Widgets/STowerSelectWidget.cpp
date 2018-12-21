// Fill out your copyright notice in the Description page of Project Settings.

#include "STowerSelectWidget.h"
#include "SlateOptMacros.h"
#include "../Style/FTowerDefenseStyle.h"
#include "../Style/NumberSlateWidgetStyle.h"
#include "../Style/TowerSelectWidgetStyle.h"
#include <Attribute.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void STowerSelectWidget::Construct(const FArguments& InArgs)
{
	StartPos = FVector2D(1280, 720);
	MoveDir = FVector2D(-1, -1);
	NumberStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FNumberSlateStyle>(TEXT("NumberStyle"));
	TowerSelectStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FTowerSelectStyle>(TEXT("TowerSelectStyle"));

	TAttribute<FMargin>::FGetter PaddingGetter;
	PaddingGetter.BindLambda([&]() {
		const float CurLerp = MoveAnim.GetLerp();
		FVector2D NewPos = StartPos + 300 * MoveDir.GetSafeNormal() * CurLerp;
		WidgetBox->SetRenderOpacity(CurLerp);
		return FMargin(NewPos.X, NewPos.Y, 0.f, 0.f);
		});

	TAttribute<FMargin> MoveAttribute;
	MoveAttribute.Bind(PaddingGetter);

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Top)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.Padding(MoveAttribute)
		[
			SAssignNew(WidgetBox, SBox)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.WidthOverride(200)
			.HeightOverride(240)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.FillHeight(5.f)
				[
					SNew(SBorder)
					.VAlign(EVerticalAlignment::VAlign_Fill)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.RenderTransformPivot(FVector2D(0.5f,0.5f))
					.RenderTransform(FSlateRenderTransform(1.2f))
					.BorderImage(&TowerSelectStyle->Border)
					[
						SNew(SButton)
						.VAlign(EVerticalAlignment::VAlign_Fill)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f)))
						[
							SNew(SImage)
							.RenderTransformPivot(FVector2D(0.5f, 0.5f))
							.RenderTransform(FSlateRenderTransform(0.9f))
							.Image(&TowerSelectStyle->TwoMissle)
						]
					]
				]
				+ SVerticalBox::Slot()
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SAssignNew(TowerCostNumbers, SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(EVerticalAlignment::VAlign_Fill)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(SBorder)
						.RenderOpacity(0.f)
					]
					+ SHorizontalBox::Slot()
					.VAlign(EVerticalAlignment::VAlign_Fill)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(SImage)
						.Image(&NumberStyle->MoneyCoin)
						.RenderTransformPivot(FVector2D(0.5f, 0.5f))
						.RenderTransform(FSlateRenderTransform(1.5f))
					]
				]
			]
		]
	];

	for (int32 i = 0; i < 4; ++i)
	{
		TowerCostNumbers->AddSlot()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SImage)
			.Image(&NumberStyle->Numbers[i])
			.RenderTransformPivot(FVector2D(0.5f, 0.5f))
			.RenderTransform(FSlateRenderTransform(1.5f))
		];
	}

	TowerCostNumbers->AddSlot()
	.VAlign(EVerticalAlignment::VAlign_Fill)
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(SBorder)
		.RenderOpacity(0.f)
	];

	SetupAnimation();
}

void STowerSelectWidget::SetupAnimation()
{
	MoveSecquence = FCurveSequence();
	MoveAnim = MoveSecquence.AddCurve(0.f, 5.f, ECurveEaseFunction::Linear);
	MoveSecquence.Play(this->AsShared());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION