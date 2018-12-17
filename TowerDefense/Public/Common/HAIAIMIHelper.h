// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TOWERDEFENSE_API HAIAIMIHelper
{
public:
	static void Debug_ScreenMessage(FString&& InString, float ShowTime = 1.f, FColor FontColor = FColor::Blue);

	static void Debug_LogMessage(FString&& InString);

	static float AdaptSubAngle(const float InAngle1, const float InAngle2);

	static float AdaptAngle(const float InAngle);
};
