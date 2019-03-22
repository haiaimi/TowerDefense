// Fill out your copyright notice in the Description page of Project Settings.

#include "SPauseMenuWidget.h"
#include "SlateOptMacros.h"
#include <SBackgroundBlur.h>
#include <Engine/Texture2D.h>
#include "TDController.h"
#include <Engine/World.h>
#include <Engine/Engine.h>
#include "HAIAIMIHelper.h"
#include <SlateTypes.h>
#include "FTowerDefenseStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	OwnerController = InArgs._OwnerController;
	UTexture2D* ButtonImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/map/Texture/towerDefense_tile015"), nullptr, LOAD_None, nullptr);
	FSlateBrush* ButtonBrush = new FSlateBrush;
	FSlateBrush* BackgroundBrush_Android = new FSlateBrush;
	ButtonBrush->SetResourceObject(ButtonImage);
	ButtonBrush->DrawAs = ESlateBrushDrawType::Box;
	ButtonBrush->Margin = FMargin(0.25);
	BackgroundBrush_Android->TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.5f));

	ButtonStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("PauseMenuButtonStyle"));
	FSimpleDelegate RestartDelegate;
	RestartDelegate.BindUObject(OwnerController.Get(), &ATDController::RestartGame);

	ChildSlot
	[
		SNew(SOverlay)
#if PLATFORM_ANDROID
		+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(BackgroundBrush_Android)    //由于安卓不支持高斯模糊
		]
#endif
#if PLATFORM_WINDOWS
		+ SOverlay::Slot()
		[
			SNew(SBackgroundBlur)
			.BlurStrength(5.f)
		]
#endif
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
						SAssignNew(ContinueButton, SButton)
						.HAlign(EHorizontalAlignment::HAlign_Center)
						.VAlign(EVerticalAlignment::VAlign_Center)
						.OnPressed(this, &SPauseMenuWidget::ContinueGame)
						.ButtonStyle(ButtonStyle)
						[
							SNew(STextBlock)
							.Text(FText::FromString(FString(TEXT("继续游戏"))))
							.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/Roboto-Regular.ttf"),34))
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
							.OnPressed(RestartDelegate)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("重新开始"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/Roboto-Regular.ttf"),34))
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
								.OnPressed(InArgs._BackDelegate)
								.ButtonStyle(ButtonStyle)
								[
									SNew(STextBlock)
									.Text(FText::FromString(FString(TEXT("返回主菜单"))))
									.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/Roboto-Regular.ttf"),34))
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
							.OnPressed(this, &SPauseMenuWidget::QuitGame)
							.ButtonStyle(ButtonStyle)
							[
								SNew(STextBlock)
								.Text(FText::FromString(FString(TEXT("退出游戏"))))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/Roboto-Regular.ttf"),34))
								.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
							]
						]
					]
				]
		]
	];
	
}

void SPauseMenuWidget::SetContinueButtonEnable(bool bEnabled)
{
	ContinueButton->SetEnabled(bEnabled);
}

void SPauseMenuWidget::ContinueGame()
{
	if (OwnerController.IsValid())
		OwnerController->SetPause(false);
}

void SPauseMenuWidget::QuitGame()
{
	if (OwnerController.IsValid())
	{
		HAIAIMIHelper::SaveScore(OwnerController->GetPlayerScore());
		OwnerController->ConsoleCommand("quit");
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

