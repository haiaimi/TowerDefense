// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuSoundWidgetStyle.h"


FMenuSoundStyle::FMenuSoundStyle()
{
}

FMenuSoundStyle::~FMenuSoundStyle()
{
}

const FName FMenuSoundStyle::TypeName(TEXT("FMenuSoundStyle"));

const FMenuSoundStyle& FMenuSoundStyle::GetDefault()
{
	static FMenuSoundStyle Default;
	return Default;
}

void FMenuSoundStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

