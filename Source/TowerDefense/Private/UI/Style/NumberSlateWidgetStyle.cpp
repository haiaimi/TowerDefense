// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberSlateWidgetStyle.h"


FNumberSlateStyle::FNumberSlateStyle()
{
}

FNumberSlateStyle::~FNumberSlateStyle()
{
}

const FName FNumberSlateStyle::TypeName(TEXT("FNumberSlateStyle"));

const FNumberSlateStyle& FNumberSlateStyle::GetDefault()
{
	static FNumberSlateStyle Default;
	return Default;
}

void FNumberSlateStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

