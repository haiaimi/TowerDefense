// Fill out your copyright notice in the Description page of Project Settings.

#include "RepairWidgetStyle.h"


FRepairStyle::FRepairStyle()
{
}

FRepairStyle::~FRepairStyle()
{
}

const FName FRepairStyle::TypeName(TEXT("FRepairStyle"));

const FRepairStyle& FRepairStyle::GetDefault()
{
	static FRepairStyle Default;
	return Default;
}

void FRepairStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

