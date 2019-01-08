// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include <Sound/SoundBase.h>
#include "TDMenuHUD.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget):
		_ButtonSound(nullptr),
		_RankSound(nullptr)
	{}
	SLATE_ARGUMENT(FSimpleDelegate, OnStart)
	SLATE_ARGUMENT(FSimpleDelegate, OnQuit)
	SLATE_ARGUMENT(TWeakObjectPtr<ATDMenuHUD>, Owner)
	SLATE_ARGUMENT(TWeakObjectPtr<class USoundBase>, ButtonSound)
	SLATE_ARGUMENT(TWeakObjectPtr<class USoundBase>, RankSound)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)override;

private:
	void SetupAnimation();

	void ShowRank();

	void BackToMenu();

	void LaunchGame();

	void QuitGame();

private:
	TWeakObjectPtr<class USoundBase> RankSound;

	TWeakObjectPtr<class USoundBase> ButtonSound;

	TWeakObjectPtr<ATDMenuHUD> Owner;

	const FButtonStyle* ButtonStyle;

	const struct FMenuSoundStyle* SoundStyle;

	TSharedPtr<SVerticalBox> RankContainer;

	TSharedPtr<SBorder> BackButton;

	TArray<FCurveHandle> RankAnims;

	FCurveHandle BlurAnim;

	FCurveSequence RankSequence;
};
