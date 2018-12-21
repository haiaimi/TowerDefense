// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SCompoundWidget.h"
#include "TDTowerBase.h"
#include "TDController.h"

/**
 * 
 */
class STowerSelectWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STowerSelectWidget):
		_CurController(nullptr),
		_CurBaseTower(nullptr)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<ATDController>, CurController)
	SLATE_ARGUMENT(FVector2D, StartPos)
	SLATE_ARGUMENT(FVector2D, MoveDir)
	SLATE_ARGUMENT(float, OffsetTime)
	SLATE_ARGUMENT(TWeakObjectPtr<ATDTowerBase>, CurBaseTower)
	SLATE_ARGUMENT(TSubclassOf<ATDTowerBase>, SpecifiedTower)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SetupAnimation();

	void BuildTower();

private:
	const struct FNumberSlateStyle* NumberStyle;

	const struct FTowerSelectStyle* TowerSelectStyle;
	
	TSharedPtr<SBox> WidgetBox;

	TSharedPtr<SHorizontalBox> TowerCostNumbers;

	FCurveHandle MoveAnim;

	FCurveSequence MoveSecquence;
	
	TWeakObjectPtr<ATDController> CurController;

	/**�ؼ�����Ļ���ƶ��ķ���*/
	FVector2D MoveDir;

	FVector2D StartPos;

	float OffsetTime;

	TWeakObjectPtr<ATDTowerBase> CurBaseTower;

	TSubclassOf<ATDTowerBase> SpecifiedTower;
};