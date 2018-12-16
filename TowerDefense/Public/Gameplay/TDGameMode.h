// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATDGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

public:
	ATDGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)override;
	
public:
};
