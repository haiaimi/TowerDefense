// Fill out your copyright notice in the Description page of Project Settings.

#include "SScoreWidget.h"
#include "SlateOptMacros.h"
#include "../Style/NumberSlateWidgetStyle.h"
#include "../Style/FTowerDefenseStyle.h"
#include <Attribute.h>
#include <SlateBrush.h>
#include "STowerSelectWidget.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SScoreWidget::Construct(const FArguments& InArgs)
{
	CurSocre = 0;
	DestScore = 0;
	UpNumbers.Init(0, 5);
	DownNumbers.Init(1, 5);
	ScrollAnim.SetNum(5);
	AnimSequence.SetNum(5);
	NumberStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FNumberSlateStyle>(TEXT("NumberStyle"));

	TArray<TAttribute<const FSlateBrush*>> UpAttributes;
	TArray<TAttribute<const FSlateBrush*>> DownAttributes;
	UpAttributes.SetNum(5);
	DownAttributes.SetNum(5);

	for (int32 i = 0; i < 5; ++i)
	{
		TAttribute<const FSlateBrush*>::FGetter Getter;
		Getter.BindLambda([i,this]() {        //此处一定要注意i是值传递，否则会引用传递，报错
			return &NumberStyle->Numbers[UpNumbers[i]];
			});
		UpAttributes[i].Bind(Getter);

		Getter.BindLambda([i,this]() {
			return &NumberStyle->Numbers[DownNumbers[i]];
			});
		DownAttributes[i].Bind(Getter);
	}

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
						SNew(SImage)
						.Image(&NumberStyle->MoneyCoin)
						.RenderTransform(FSlateRenderTransform(1.8f))
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
				.Image(UpAttributes[i])
				.RenderTransform(FSlateRenderTransform(1.8f))
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
				[
					SNew(SImage)
					.Image(DownAttributes[i])
					.RenderTransform(FSlateRenderTransform(1.8f))
					.RenderOpacity(0.f)
				]
		];
	}
	SetupAnimation();
}


void SScoreWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (CurSocre == DestScore)return;      //分数相同就直接跳过下面的步骤
	FChildren* ChildrenUp = ScoreNumsUp->GetChildren();
	FChildren* ChildrenDown = ScoreNumsDown->GetChildren();

	for (int32 i = 0; i < 5; ++i)
	{
		const float CurLerp = ScrollAnim[i].GetLerp();
		if (UpNumbers[i] == GetSpecifiedNumber(i))
			continue;
		else if (AnimSequence[i].IsAtStart())
			AnimSequence[i].Play(this->AsShared());

		TPanelChildren<SBoxPanel::FSlot>* PanelChildrenUp = (TPanelChildren<SBoxPanel::FSlot>*)ChildrenUp;
		SBoxPanel::FSlot& CurSlotUp = (*PanelChildrenUp)[i + 1];

		TPanelChildren<SBoxPanel::FSlot>* PanelChildrenDown = (TPanelChildren<SBoxPanel::FSlot>*)ChildrenDown;
		SBoxPanel::FSlot& CurSlotDown = (*PanelChildrenDown)[i + 1];
		CurSlotUp.GetWidget()->SetRenderOpacity(1.f - CurLerp);
		CurSlotDown.GetWidget()->SetRenderOpacity(CurLerp);
		CurSlotUp.GetWidget()->SetRenderTransform(FVector2D(0.f, -50.f*CurLerp));
		CurSlotDown.GetWidget()->SetRenderTransform(FVector2D(0.f, -50.f*CurLerp));

		if (AnimSequence[i].IsAtEnd())       //一个轮回结束
		{
			UpNumbers[i] = (UpNumbers[i] + 1) % 10;
			DownNumbers[i] = (DownNumbers[i] + 1) % 10;

			CurSlotUp.GetWidget()->SetRenderOpacity(1.f);
			CurSlotDown.GetWidget()->SetRenderOpacity(0.f);
			CurSlotUp.GetWidget()->SetRenderTransform(FVector2D(0.f, 0.f));
			CurSlotDown.GetWidget()->SetRenderTransform(FVector2D(0.f, 0.f));
			CurSocre += FMath::Pow(10, 4 - i);
			AnimSequence[i].JumpToStart();
		}
	}
}

void SScoreWidget::SetupAnimation()
{
	for (int32 i = 0; i < AnimSequence.Num(); ++i)
	{
		AnimSequence[i] = FCurveSequence();

		ScrollAnim[i] = AnimSequence[i].AddCurve(0.f, 0.1f, ECurveEaseFunction::Linear);
		AnimSequence[i].Play(this->AsShared());
	}
}

void SScoreWidget::AddScore(int32 AddedScore)
{
	DestScore += AddedScore;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
