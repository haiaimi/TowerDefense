// Fill out your copyright notice in the Description page of Project Settings.

#include "FTowerDefenseStyle.h"
#include <SlateGameResources.h>

TSharedPtr< class FSlateStyleSet > FTowerDefenseStyle::TowerDefenseStyleInstance = NULL;

void FTowerDefenseStyle::Initialize()
{
	//≥ı ºªØSlateStyleSet
	if (!TowerDefenseStyleInstance.IsValid())
	{
		TowerDefenseStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*TowerDefenseStyleInstance);
	}
}

void FTowerDefenseStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*TowerDefenseStyleInstance);
	ensure(TowerDefenseStyleInstance.IsUnique());
	TowerDefenseStyleInstance.Reset();
}

FName FTowerDefenseStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("TowerDefenseStyle"));
	return StyleSetName;
}


TSharedRef< class FSlateStyleSet > FTowerDefenseStyle::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FTowerDefenseStyle::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	FSlateStyleSet& Style = StyleRef.Get();

	return StyleRef;
}

const ISlateStyle& FTowerDefenseStyle::Get()
{
	return *TowerDefenseStyleInstance;
}