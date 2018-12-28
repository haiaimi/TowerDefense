// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/TDEnemy.h"
#include "TDEnemy_Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATDEnemy_Tank : public ATDEnemy
{
	GENERATED_BODY()
	
public:
	/**炮管Sprites*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPaperSpriteComponent* TankBarrel;
	
	/**坦克开火的火焰*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPaperSpriteComponent* TankFire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapon")
	TSubclassOf<class ATDProjectile> TankProjectile;

	/**火焰持续时间*/
	FTimerHandle FireTime;

	/**开火间隙*/
	FTimerHandle FireInterval;

	int32 TowerIndex;

	float TankPitch;

public:
	ATDEnemy_Tank();

	virtual void BeginPlay()override;

	virtual void PostInitializeComponents()override;

	virtual void Tick(float DeltaTime)override;

	/**坦克开火*/
	void Fire();

	/**开火循环，2秒内发射一次*/
	void FireLoop();

	/**获取进的炮台位置*/
	FVector GetNearestTower();
};
