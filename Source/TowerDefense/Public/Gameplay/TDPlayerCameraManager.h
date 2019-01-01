// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TDPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATDPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ATDPlayerCameraManager();

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)override;

private:
	FMinimalViewInfo Temp;
};
