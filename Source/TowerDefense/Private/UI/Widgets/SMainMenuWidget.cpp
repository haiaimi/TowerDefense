// Fill out your copyright notice in the Description page of Project Settings.

#include "SMainMenuWidget.h"
#include "SlateOptMacros.h"
#include <UObjectGlobals.h>
#include <Engine/Texture2D.h>

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
	
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
