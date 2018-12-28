// Fill out your copyright notice in the Description page of Project Settings.

#include "STowerSelectWidget.h"
#include "SlateOptMacros.h"
#include "../Style/FTowerDefenseStyle.h"
#include "../Style/NumberSlateWidgetStyle.h"
#include "../Style/TowerSelectWidgetStyle.h"
#include <Attribute.h>
#include <Engine/World.h>
#include <Engine/Engine.h>
#include <Kismet/GameplayStatics.h>
#include "SScoreWidget.h"
#include "STowerButtonWidget.h"

static const TArray<FVector2D> TowerWidgetDirs = { FVector2D(-1,-1),FVector2D(0,-1),FVector2D(1,-1) };

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void STowerSelectWidget::Construct(const FArguments& InArgs)
{
	CurController = InArgs._CurController;
	StartPos = InArgs._StartPos;
	CurBaseTower = InArgs._CurBaseTower;

	TowerButtons.SetNum(3);
	MoveAnim.SetNum(3);
	NumberStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FNumberSlateStyle>(TEXT("NumberStyle"));
	TowerSelectStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FTowerSelectStyle>(TEXT("TowerSelectStyle"));

	TArray<TAttribute<FMargin>> MoveAttribute;
	TArray<TAttribute<TOptional<FSlateRenderTransform>>> ButtonScales;
	MoveAttribute.SetNum(3);
	ButtonScales.SetNum(3);
	for (int32 i = 0; i < 3; ++i)
	{
		TAttribute<FMargin>::FGetter PaddingGetter;
		TAttribute<TOptional<FSlateRenderTransform>>::FGetter ScaleGetter;
		PaddingGetter.BindLambda([i,this]() {
			const float CurLerp = MoveAnim[i].GetLerp();
			FVector2D NewPos = StartPos + 210 * TowerWidgetDirs[i].GetSafeNormal() * CurLerp;
			
			const float Scale = 0.5f + 0.5f*CurLerp;
			TowerButtons[i]->SetRenderOpacity(CurLerp);
			return FMargin(NewPos.X - 75, NewPos.Y - 90, 0.f, 0.f);
			});

		ScaleGetter.BindLambda([i,this]() {
			const float Scale = 1.2f*MoveAnim[i].GetLerp();
			if (CurController.IsValid() && MoveSecquence.IsInReverse() && MoveSecquence.IsAtStart())
				CurController->ResetSelectWidget();
			return FSlateRenderTransform(Scale);
			});

		MoveAttribute[i].Bind(PaddingGetter);
		ButtonScales[i].Bind(ScaleGetter);
	}

	TArray<FOnClicked> ClickEvent;
	ClickEvent.SetNum(3);
	for (int32 i = 0; i < 3; ++i)
	{
		ClickEvent[i].BindLambda([i, this]() {

			if (CurBaseTower.IsValid() && CurController.IsValid())
			{
				if (CurController->SpawnTower(0, CurBaseTower.Get()))
				{
					CurBaseTower.Reset();
					MoveSecquence.Reverse();
				}
			}
			return FReply::Handled();
			});
	}

	ChildSlot
	[
		SAssignNew(ButtonsOverlay, SOverlay)
	];

	for (int32 i = 0; i < 3; ++i)
	{
		FSlateBrush* BorderImage = const_cast<FSlateBrush*>(&TowerSelectStyle->Border);
		FSlateBrush* TowerImage = const_cast<FSlateBrush*>(&TowerSelectStyle->TowerImages[i]);
		ButtonsOverlay->AddSlot()
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Top)
			.Padding(MoveAttribute[i])
			[
				SAssignNew(TowerButtons[i], STowerButtonWidget)
				.ButtonScale(ButtonScales[i])
				.OnClicked(ClickEvent[i])
				.TowerCost(CurController.IsValid()?CurController->GetSpecifiedTowerCost(i):300)
				.BorderImage(BorderImage)
				.TowerImage(TowerImage)
			];
	}

	SetupAnimation();
}

void STowerSelectWidget::SetupAnimation()
{
	MoveSecquence = FCurveSequence();

	for (int32 i = 0; i < 3; ++i)
	{
		MoveAnim[i] = MoveSecquence.AddCurve(0.1f*i, 0.3f, ECurveEaseFunction::QuadInOut);
		MoveSecquence.Play(this->AsShared());
	}
}

void STowerSelectWidget::ReserveButtons()
{
	if (!MoveSecquence.IsInReverse())
		MoveSecquence.Reverse();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
