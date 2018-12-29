﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenuWidget.h"
#include "SlateOptMacros.h"
#include <UObjectGlobals.h>
#include <Engine/Texture2D.h>
#include "SBackgroundBlur.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	UTexture2D* BackgroundImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/BattleField"), nullptr, LOAD_None, nullptr);
	UTexture2D* BorderImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Enemy/Smoke/smokeGrey0"), nullptr, LOAD_None, nullptr);
	FSlateBrush* BackgroundBrush = new FSlateBrush;
	FSlateBrush* BorderBrush = new FSlateBrush;
	BackgroundBrush->SetResourceObject(BackgroundImage);
	BorderBrush->SetResourceObject(BorderImage);
	BorderBrush->DrawAs = ESlateBrushDrawType::Box;
	BorderBrush->Margin = FMargin(0.25);

	RankAnims.SetNum(11);
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		[
			SNew(SBox)
			.HeightOverride(1080)
			.WidthOverride(1920)
			[
				SNew(SImage)
				.Image(BackgroundBrush)
			]
		]
		+SOverlay::Slot()
		[
			SNew(SBackgroundBlur)
			.BlurStrength(5.f)
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(FMargin(150.f,100.f,0.f,0.f))
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(280.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("继续游戏"))))
							.Font(FSlateFontInfo("Roboto",34))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.105076f,0.251329f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
				.RenderTransform(FSlateRenderTransform(FVector2D(0.f,0.f)))
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(280.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						.OnPressed(InArgs._OnPressed)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("开始游戏"))))
							.Font(FSlateFontInfo("Roboto",34))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.105076f,0.251329f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(280.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						.OnPressed(this, &SMainMenuWidget::ShowRank)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("排行榜"))))
							.Font(FSlateFontInfo("Roboto",34))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.105076f,0.251329f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(280.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("退出游戏"))))
							.Font(FSlateFontInfo("Roboto",34))
							.ColorAndOpacity(FSlateColor(FLinearColor(0.105076f,0.251329f,1.f,1.f)))
						]
					]
				]
			]
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SAssignNew(RankContainer,SVerticalBox)
		]
	];

	TArray<TAttribute<TOptional<FSlateRenderTransform>>> RankAttributes;
	TAttribute<TOptional<FSlateRenderTransform>>::FGetter TransformGetter;
	RankAttributes.SetNum(11);

	for (int32 i = 0; i < 11; ++i)
	{
		TransformGetter.BindLambda([i, this]() {
			float CurLerp = RankAnims[i].GetLerp();
			return FSlateRenderTransform(FVector2D(1350.f-CurLerp*1350.f, 0.f));			
			});
		RankAttributes[i].Bind(TransformGetter);
	}

	RankContainer->AddSlot()
	.HAlign(EHorizontalAlignment::HAlign_Center)
	.VAlign(EVerticalAlignment::VAlign_Center)
	[
		SNew(SBorder)
		.BorderImage(BorderBrush)
		.RenderTransform(RankAttributes[0])
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.VAlign(EVerticalAlignment::VAlign_Center)
			.HeightOverride(80.f)
			.WidthOverride(300.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString(TEXT("排行榜"))))
				.Font(FSlateFontInfo("Roboto",34))
				.ColorAndOpacity(FSlateColor(FLinearColor(0.105076f,0.251329f,1.f,1.f)))
			]
		]
	];

	//添加Rank条
	for (int32 i = 0; i < 10; ++i)
	{
		RankContainer->AddSlot()
		[
			SNew(SBorder)
			.BorderImage(BorderBrush)
			.RenderTransform(RankAttributes[i+1])
			[
				SNew(SBox)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.HeightOverride(80.f)
				.WidthOverride(700.f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString(TEXT("111111111"))))
					.Font(FSlateFontInfo("Roboto",34))
					.ColorAndOpacity(FSlateColor(FLinearColor(0.105076f,0.251329f,1.f,1.f)))
				]
			]
		];

		SetupAnimation();
	}
}

void SMainMenuWidget::SetupAnimation()
{
	RankSequence = FCurveSequence();
	float StartTime = 0.f;

	for (int32 i = 0; i < 11; ++i)
	{
		RankAnims[i] = RankSequence.AddCurve(i * 0.05f, 0.5f, ECurveEaseFunction::Linear);
	}
}

void SMainMenuWidget::ShowRank()
{
	if (RankSequence.IsAtStart())
		RankSequence.Play(this->AsShared());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION