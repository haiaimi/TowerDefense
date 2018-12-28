// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerDefense.h"
#include "Modules/ModuleManager.h"
#include "Private/UI/Style/FTowerDefenseStyle.h"

class FTowerDefenseGameModule :public FDefaultGameModuleImpl
{
	virtual void StartupModule()override
	{
		//加载样式
		FSlateStyleRegistry::UnRegisterSlateStyle(FTowerDefenseStyle::GetStyleSetName());
		FTowerDefenseStyle::Initialize();
	}

	virtual void ShutdownModule()override
	{
		//卸载样式
		FTowerDefenseStyle::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FTowerDefenseGameModule, TowerDefense, "TowerDefense" );

DEFINE_LOG_CATEGORY(LogHM)