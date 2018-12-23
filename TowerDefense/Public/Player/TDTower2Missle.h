// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TDTowerBase.h"
#include "TDTower2Missle.generated.h"

/**
 *  带有两个火箭弹的炮台
 */
UCLASS()
class TOWERDEFENSE_API ATDTower2Missle : public ATDTowerBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* TowerBarrel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AMissle> MissleType;

	/**火箭弹*/
	class TArray<class AMissle*> Missles;

	/**开火间隙*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireInterval;

	float TowerBarrelPitch;

	/**开火定时器*/
	FTimerHandle ReloadTimer;

public:
	ATDTower2Missle();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	/**重新装弹*/
	void Reload();

	void FireLoop();

	void Fire();

	FTransform GetNearestEnemy();

	virtual void Destroyed()override;

	virtual void OnInjured()override;
};
