// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TDTowerBase.h"
#include "TDTowerMissle.generated.h"

/**
 *  带有两个火箭弹的炮台
 */
UCLASS()
class TOWERDEFENSE_API ATDTowerMissle : public ATDTowerBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* TowerBarrel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ATDProjectile> WeaponType;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* FireSound;

	/**火箭弹*/
	class TArray<class AMissle*> Missles;

	/**开火间隙*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireInterval;

	float TowerBarrelPitch;

	/**开火定时器*/
	FTimerHandle ReloadTimer;

public:
	ATDTowerMissle();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	/**重新装弹*/
	virtual void Reload();

	virtual void FireLoop();

	virtual void Fire();

	FTransform GetNearestEnemy();

	virtual void Destroyed()override;

	virtual void OnInjured()override;
};
