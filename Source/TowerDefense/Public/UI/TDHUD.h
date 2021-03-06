// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TDHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATDHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATDHUD();

	virtual void DrawHUD()override;

	virtual void BeginPlay()override;

	TSharedPtr<class SHUDWidget> GetHUDWidget()
	{
		return ScoreWidget;
	}

	void SpawnBomber();

private:
	TSharedPtr<class SHUDWidget> ScoreWidget;

	TSharedPtr<class SPauseMenuWidget> PauseWidget;
};
