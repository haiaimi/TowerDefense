// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerSelectWidgetStyle.h"


FTowerSelectStyle::FTowerSelectStyle()
{
}

FTowerSelectStyle::~FTowerSelectStyle()
{
}

const FName FTowerSelectStyle::TypeName(TEXT("FTowerSelectStyle"));

const FTowerSelectStyle& FTowerSelectStyle::GetDefault()
{
	static FTowerSelectStyle Default;
	return Default;
}

void FTowerSelectStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

