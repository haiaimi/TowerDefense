// Fill out your copyright notice in the Description page of Project Settings.

#include "TDPlayerCameraManager.h"
#include "HAIAIMIHelper.h"


ATDPlayerCameraManager::ATDPlayerCameraManager()
{
	bAlwaysApplyModifiers = true;
	Temp.ProjectionMode = ECameraProjectionMode::Orthographic;
	Temp.AspectRatio = 1.777778f;
	Temp.OrthoWidth = 2300.f;
	Temp.Location = FVector(1130.f, 2210.f, -630.f);
	Temp.Rotation = FRotator(0.f, -90.f, 0.f);
}

void ATDPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	OutVT.POV = Temp;
}
