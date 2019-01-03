// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TDProjectile.h"
#include "TowerBullet.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATowerBullet : public ATDProjectile
{
	GENERATED_BODY()

public:
	ATowerBullet();
	
	UFUNCTION()
	virtual void OnImpact(const FHitResult& result)override;
};
