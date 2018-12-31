﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "SPauseMenuWidget.h"
#include "SlateOptMacros.h"
#include <SBackgroundBlur.h>
#include <Engine/Texture2D.h>
#include "TDController.h"
#include <Engine/World.h>
#include <Engine/Engine.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerController = InArgs._OwnerController;
	UTexture2D* ButtonImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/map/Texture/towerDefense_tile015"), nullptr, LOAD_None, nullptr);
	FSlateBrush* ButtonBrush = new FSlateBrush;
	ButtonBrush->SetResourceObject(ButtonImage);
	ButtonBrush->DrawAs = ESlateBrushDrawType::Box;
	ButtonBrush->Margin = FMargin(0.25);

	FSimpleDelegate RestartDelegate;
	RestartDelegate.BindUObject(OwnerController.Get(), &ATDController::RestartGame);

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SBackgroundBlur)
			.BlurStrength(5)
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Center)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SBorder)
				.BorderImage(ButtonBrush)
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(400.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
						.OnPressed(this, &SPauseMenuWidget::ContinueGame)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("继续游戏"))))
							.Font(FSlateFontInfo("Roboto",34))
							.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
						]
					]
				]
			]
			+SVerticalBox::Slot()
				[
					SNew(SBorder)
					.BorderImage(ButtonBrush)
					[
						SNew(SBox)
						.HeightOverride(100.f)
						.WidthOverride(400.f)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
							.OnPressed(RestartDelegate)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("重新开始"))))
								.Font(FSlateFontInfo("Roboto",34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
				]
				+SVerticalBox::Slot()
					[
						SNew(SBorder)
						.BorderImage(ButtonBrush)
						[
							SNew(SBox)
							.HeightOverride(100.f)
							.WidthOverride(400.f)
							[
								SNew(SButton)
								.HAlign(EHorizontalAlignment::HAlign_Center)
								.VAlign(EVerticalAlignment::VAlign_Center)
								.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
								.OnPressed(InArgs._BackDelegate)
								[
									SNew(STextBlock)
									.Text(FText::FromString(FString(TEXT("返回主菜单"))))
									.Font(FSlateFontInfo("Roboto",34))
									.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
								]
							]
						]
					]
				+SVerticalBox::Slot()
				[
					SNew(SBorder)
					.BorderImage(ButtonBrush)
					[
						SNew(SBox)
						.HeightOverride(100.f)
						.WidthOverride(400.f)
						[
							SNew(SButton)
							.HAlign(EHorizontalAlignment::HAlign_Center)
							.VAlign(EVerticalAlignment::VAlign_Center)
							.ButtonColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,0.f)))
							.OnPressed(this, &SPauseMenuWidget::QuitGame)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("退出游戏"))))
								.Font(FSlateFontInfo("Roboto",34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
			]
		]
	];
	
}
void SPauseMenuWidget::ContinueGame()
{
	if (OwnerController.IsValid())
		OwnerController->SetPause(false);
}

void SPauseMenuWidget::QuitGame()
{
	if (OwnerController.IsValid())
		OwnerController->ConsoleCommand("quit");
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
