// Fill out your copyright notice in the Description page of Project Settings.

#include "SScoreWidget.h"
#include "SlateOptMacros.h"
#include "../Style/NumberSlateWidgetStyle.h"
#include "../Style/FTowerDefenseStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SScoreWidget::Construct(const FArguments& InArgs)
{
	UpImages.SetNum(5);
	DownImages.SetNum(5);
	NumberStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FNumberSlateStyle>(TEXT("NumberStyle"));

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(EVerticalAlignment::VAlign_Top)
			.HAlign(EHorizontalAlignment::HAlign_Right)
			.Padding(FMargin(0.f,0.f,50.f,0.f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SAssignNew(ScoreNumsUp, SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(EVerticalAlignment::VAlign_Fill)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.AutoWidth()
					[
						SNew(SBox)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.VAlign(EVerticalAlignment::VAlign_Fill)
						.WidthOverride(50)
						.HeightOverride(50)
						[
							SAssignNew(TempImage, SImage)
							.Image(&NumberStyle->MoneyCoin)
							.RenderTransform(FSlateRenderTransform(2.f))
						]
					]
				]
				+SVerticalBox::Slot()
				[
					SAssignNew(ScoreNumsDown, SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(EVerticalAlignment::VAlign_Fill)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.AutoWidth()
					[
						SNew(SBox)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.VAlign(EVerticalAlignment::VAlign_Fill)
						.WidthOverride(50)
						.HeightOverride(50)
					]
				]
				
		]
	];

	for (int32 i = 0; i < 5; ++i)
	{
		ScoreNumsUp->AddSlot()
		.VAlign(EVerticalAlignment::VAlign_Fill)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.AutoWidth()
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.WidthOverride(50)
			.HeightOverride(50)
			.RenderTransform(FSlateRenderTransform(FVector2D(0.f,0.f)))
			[
				SNew(SImage)
				.Image(&NumberStyle->Numbers[0])
				.RenderTransform(FSlateRenderTransform(2.f))
			]
		];

			ScoreNumsDown->AddSlot()
			.VAlign(EVerticalAlignment::VAlign_Fill)
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.AutoWidth()
			[
				SNew(SBox)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				.WidthOverride(50)
				.HeightOverride(50)
				.RenderTransform(FSlateRenderTransform(FVector2D(0.f, 10.f)))
				[
					SNew(SImage)
					.Image(&NumberStyle->Numbers[0])
					.RenderTransform(FSlateRenderTransform(2.f))
				]
		];
	}

}


void SScoreWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	static float alpha = 0.f;
	alpha += InDeltaTime * 0.3f;

	TempImage->SetRenderOpacity(alpha);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION