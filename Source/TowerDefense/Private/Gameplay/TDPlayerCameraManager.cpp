// Fill out your copyright notice in the Description page of Project Settings.

#include "TDPlayerCameraManager.h"
#include "HAIAIMIHelper.h"




ATDPlayerCameraManager::ATDPlayerCameraManager()
{
	bAlwaysApplyModifiers = true;
	Temp.ProjectionMode = ECameraProjectionMode::Orthographic;
	Temp.AspectRatio = 1.777778f;
	Temp.OrthoWidth = 1200.f;
	Temp.Location = FVector(590.f, 1270.f, -590.f);
	Temp.Rotation = FRotator(0.f, -90.f, 0.f);
}

void ATDPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	OutVT.POV = Temp;
	HAIAIMIHelper::Debug_ScreenMessage(Temp.Location.ToString());
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);
}
