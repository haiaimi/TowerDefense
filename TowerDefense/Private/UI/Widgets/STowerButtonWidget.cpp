// Fill out your copyright notice in the Description page of Project Settings.

#include "STowerButtonWidget.h"
#include "SlateOptMacros.h"
#include "../Style/FTowerDefenseStyle.h"
#include "../Style/NumberSlateWidgetStyle.h"
#include "../Style/TowerSelectWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void STowerButtonWidget::Construct(const FArguments& InArgs)
{
	NumberStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FNumberSlateStyle>(TEXT("NumberStyle"));
	TowerSelectStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FTowerSelectStyle>(TEXT("TowerSelectStyle"));

	TAttribute<TOptional<FSlateRenderTransform>> ScaleAttribute;
	const int32 TowerCost = InArgs._TowerCost;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(EVerticalAlignment::VAlign_Top)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.WidthOverride(150)
			.HeightOverride(180)
			
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
					.RenderTransformPivot(FVector2D(0.5f, 0.5f))
					.RenderTransform(InArgs._ButtonScale)
					.BorderImage(&TowerSelectStyle->Border)
					[
						SNew(SButton)
						.VAlign(EVerticalAlignment::VAlign_Fill)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f)))
						.OnClicked(InArgs._OnClicked)
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
				.Image(&NumberStyle->Numbers[GetSpecifiedNumber(TowerCost, i)])
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
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION