// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ScoreSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API UScoreSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	
public:
	/**�洢���ǰʮ�ķ���*/
	UPROPERTY()
	TArray<uint32> AllScores;
	
};
