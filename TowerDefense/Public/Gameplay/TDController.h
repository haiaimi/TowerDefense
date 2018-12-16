// Fill out your copyright notice in the Description page of Project Settings.

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

public:
	ATDController();

	void BeginPlay()override;

	virtual void SetupInputComponent()override;

	void SpawnEnemy();
};
