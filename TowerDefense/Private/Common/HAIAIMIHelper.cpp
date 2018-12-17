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

float HAIAIMIHelper::AdaptSubAngle(const float InAngle1, const float InAngle2)
{
	const float TempSub = InAngle1 - InAngle2;

	if (TempSub > 180.f)
		return 360.f - TempSub;
	else if (TempSub < -180.f)
		return 360.f + TempSub;
	else return TempSub;
}

float HAIAIMIHelper::AdaptAngle(const float InAngle)
{
	if (InAngle > 180.f)
		return 360.f - InAngle;
	else if (InAngle < -180.f)
		return 360.f + InAngle;
	else return InAngle;
}
