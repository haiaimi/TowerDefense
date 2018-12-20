﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDController.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATDController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATDMap* CurMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATDMap> DefaultMap;

	TSubclassOf<class ATDTowerBase> Tower1;

public:
	ATDController();

	void BeginPlay()override;

	virtual void SetupInputComponent()override;

	void SpawnEnemy();

	/**点击事件检测地图*/
	void DetectMap();

	void AddScore(int32 AddedScore);
};
