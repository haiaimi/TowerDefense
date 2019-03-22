// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TDProjectile.h"
#include "TankBullet.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATankBullet : public ATDProjectile
{
	GENERATED_BODY()
	
	
public:
	ATankBullet();

	virtual void OnImpact(const FHitResult& result)override;
};
