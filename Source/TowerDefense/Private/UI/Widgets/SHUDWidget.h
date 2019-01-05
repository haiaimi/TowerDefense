// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"

/**
 *
 */
class SHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHUDWidget)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class ATDHUD>, MyHUD)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)override;

	void SetupAnimation();

	FORCEINLINE int32 GetSpecifiedNumber(int32 Index)
	{
		int32 Res = (DestScore / (FMath::Pow(10, 4 - Index)));
		return Res % 10;
	}

	void AddScore(int32 AddedScore);

	/** 
	  * Method:    AddRepairWidget  添加修理控件
	  * FullName:  SHUDWidget::AddRepairWidget
	  * @Parameter: FVector2D WidgetPos  控件生成位置 
	  * @Parameter: class ATDTowerBase * Tower  所对应的Tower
	  * @Returns:  TSharedPtr<SRepairWidget> 返回生成的控件
	  */
	TSharedPtr<class SRepairWidget> AddRepairWidget(FVector2D WidgetPos, class ATDTowerBase* Tower);

	/**删除对应的修理控件*/
	void RemoveRepairWidget(TSharedPtr<class SRepairWidget> AimWidget);

private:
	bool IsBoomReady()const;

	TOptional<float> GetBoomPercent()const;

	/**呼叫空援*/
	void CallAirSupport();

private:
	/**面板当前显示的分数*/
	int32 CurSocre;

	/**目标分数*/
	int32 DestScore;

	const struct FNumberSlateStyle* NumberStyle;

	const struct FProgressBarStyle* ProgressStyle;

	const struct FButtonStyle* BombStyle;

	TSharedPtr<SOverlay> HUDOverlay;

	TSharedPtr<SHorizontalBox> ScoreNumsUp;

	TSharedPtr<SHorizontalBox> ScoreNumsDown;

	TSharedPtr<SHorizontalBox> BoomTimes;

	TSharedPtr<SVerticalBox> BoomButton;

	TArray<FCurveSequence> AnimSequence;

	TArray<FCurveHandle> ScrollAnim;

	TArray<int32> UpNumbers;

	TArray<int32> DownNumbers;

	TWeakObjectPtr<class ATDHUD> MyHUD;

	//轰炸是否已经准备好
	bool bBoomReady;

	int32 RemainBoom;

	float BoomReloadTime;

	float BoomReloadTimer;
};
