// Fill out your copyright notice in the Description page of Project Settings.

#include "Common/HAIAIMIHelper.h"
#include "TowerDefense.h"
#include <Engine/Engine.h>


void HAIAIMIHelper::Debug_ScreenMessage(FString&& InString, float ShowTime, FColor FontColor)
{
	GEngine->AddOnScreenDebugMessage(-1, ShowTime, FontColor, InString);
}

void HAIAIMIHelper::Debug_LogMessage(FString&& InString)
{
	UE_LOG(LogHM, Log, TEXT("%s"), *InString);
}