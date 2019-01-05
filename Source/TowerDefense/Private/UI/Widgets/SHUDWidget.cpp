// Fill out your copyright notice in the Description page of Project Settings.

#include "SHUDWidget.h"
#include "SlateOptMacros.h"
#include "../Style/NumberSlateWidgetStyle.h"
#include "../Style/FTowerDefenseStyle.h"
#include <Attribute.h>
#include <SlateBrush.h>
#include "STowerSelectWidget.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include <SBoxPanel.h>
#include <Engine/Texture2D.h>
#include "TDHUD.h"
#include "SRepairWidget.h"
#include "HAIAIMIHelper.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHUDWidget::Construct(const FArguments& InArgs)
{
	MyHUD = InArgs._MyHUD;
	bBoomReady = false;
	CurSocre = 0;
	DestScore = 0;
	RemainBoom = 4;
	BoomReloadTime = 5.f;
	BoomReloadTimer = 0.f;
	UpNumbers.Init(0, 5);
	DownNumbers.Init(1, 5);
	ScrollAnim.SetNum(5);
	AnimSequence.SetNum(5);
	NumberStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FNumberSlateStyle>(TEXT("NumberStyle"));
	ProgressStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FProgressBarStyle>(TEXT("BombProgressStyle"));
	BombStyle = &FTowerDefenseStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("PauseMenuButtonStyle"));   //这里直接使用暂停按钮的样式

	UTexture2D* BorderImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/map/Texture/TowerDefense_tile015"), nullptr, LOAD_None, nullptr);
	FSlateBrush* BorderBrush = new FSlateBrush;
	BorderBrush->SetResourceObject(BorderImage);

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
		SAssignNew(HUDOverlay, SOverlay)
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
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		.Padding(FMargin(0.f,50.f,0.f,0.f))
		[
			SAssignNew(BoomTimes,SHorizontalBox)
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Center)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SAssignNew(BoomButton, SVerticalBox)
			.RenderOpacity(0.5f)
			.IsEnabled(this, &SHUDWidget::IsBoomReady)
			+SVerticalBox::Slot()
			.FillHeight(4.5f)
			[
				SNew(SBorder)
				.BorderImage(BorderBrush)
				.RenderTransform(FSlateRenderTransform(1.2f))
				.RenderTransformPivot(FVector2D(0.5f,0.5f))
				[
					SNew(SBox)
					.WidthOverride(130.f)
					.HeightOverride(130.f)
					[
						SNew(SButton)
						.HAlign(EHorizontalAlignment::HAlign_Fill)
						.VAlign(EVerticalAlignment::VAlign_Fill)
						.ButtonStyle(BombStyle)
						.OnPressed(this, &SHUDWidget::CallAirSupport)
						[
							SNew(SProgressBar)
							.Style(ProgressStyle)
							.Percent(this, &SHUDWidget::GetBoomPercent)
							.BarFillType(EProgressBarFillType::TopToBottom)
							.RenderTransform(FSlateRenderTransform(FMatrix2x2(FQuat2D(-0.5f*PI)).Concatenate(FMatrix2x2(0.8f))))
							.RenderTransformPivot(FVector2D(0.5f,0.5f))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.HAlign(EHorizontalAlignment::HAlign_Center)
			.FillHeight(1.f)
			[
				SNew(SBox)
				.WidthOverride(130.f)
				.HeightOverride(30.f)
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text(FText::FromString(FString(TEXT("呼叫空援"))))
					.Font(FSlateFontInfo("Roboto",18))
				]
				
			]
		]
	];

	//填充轰炸次数图标
	for (int32 i = 0; i < RemainBoom; ++i)
	{
		BoomTimes->AddSlot()
		[
			SNew(SBox)
			.WidthOverride(80.f)
			.HeightOverride(80.f)
			[
				SNew(SImage)
				.Image(&ProgressStyle->FillImage)
				.RenderTransform(FSlateRenderTransform(FQuat2D(-0.25f*PI)))
			]
		];
	}

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


void SHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	static float BreatheTime = 0.f;
	BreatheTime += InDeltaTime;

	if ((BoomReloadTimer / BoomReloadTime) >= 1.f&&RemainBoom > 0)bBoomReady = true;
	if (!bBoomReady)
	{
		BoomButton->SetRenderOpacity(1.f);
		if (RemainBoom > 0)
			BoomReloadTimer += InDeltaTime;
		else
			BoomReloadTimer = BoomReloadTime;
	}
	else
	{
		int32 Tmp = static_cast<int32>(BreatheTime);
		if (Tmp % 2)
			BoomButton->SetRenderOpacity(BreatheTime - Tmp);
		else
			BoomButton->SetRenderOpacity(1.f - (BreatheTime - Tmp));
	}

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

void SHUDWidget::SetupAnimation()
{
	for (int32 i = 0; i < AnimSequence.Num(); ++i)
	{
		AnimSequence[i] = FCurveSequence();

		ScrollAnim[i] = AnimSequence[i].AddCurve(0.f, 0.1f, ECurveEaseFunction::Linear);
		AnimSequence[i].Play(this->AsShared());
	}
}

void SHUDWidget::AddScore(int32 AddedScore)
{
	DestScore += AddedScore;
}

TSharedPtr<class SRepairWidget> SHUDWidget::AddRepairWidget(FVector2D WidgetPos, class ATDTowerBase* Tower)
{
	TSharedPtr<SRepairWidget> RepairWidget(nullptr);
	HUDOverlay->AddSlot(0)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SAssignNew(RepairWidget, SRepairWidget)
			.SpawnPos(WidgetPos)
			.TowerBase(Tower)
		];
	FChildren* Children = HUDOverlay->GetChildren();
	TPanelChildren<SOverlay::FOverlaySlot>* PanelChildren = (TPanelChildren<SOverlay::FOverlaySlot>*)Children;
	(*PanelChildren)[PanelChildren->Num()-1].ZOrder = 2;
	(*PanelChildren)[2].ZOrder = 2;
	/*for (int32 i = 0; i < PanelChildren->Num(); ++i)
	{
		HAIAIMIHelper::Debug_ScreenMessage(FString::FormatAsNumber((*PanelChildren)[i].ZOrder),2.f);
	}*/
	return RepairWidget;
}

void SHUDWidget::RemoveRepairWidget(TSharedPtr<class SRepairWidget> AimWidget)
{
	HUDOverlay->RemoveSlot(AimWidget.ToSharedRef());
}

bool SHUDWidget::IsBoomReady()const
{
	return bBoomReady;
}

TOptional<float> SHUDWidget::GetBoomPercent() const
{
	return BoomReloadTimer / BoomReloadTime;
}

void SHUDWidget::CallAirSupport()
{
	RemainBoom--;
	BoomReloadTimer = 0.f;
	bBoomReady = false;
	
	if (RemainBoom < 0)return;
	FChildren* Children = BoomTimes->GetChildren();
	TPanelChildren<SBoxPanel::FSlot>* PanelChildren = (TPanelChildren<SBoxPanel::FSlot>*)Children;
	BoomTimes->RemoveSlot((*PanelChildren)[RemainBoom].GetWidget());

	if (MyHUD.IsValid())
		MyHUD->SpawnBomber();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION