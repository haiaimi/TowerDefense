// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TDTowerBase.h"
#include "TDTowerMissle.generated.h"

/**
 *  �����������������̨
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

	/**�����*/
	class TArray<class AMissle*> Missles;

	/**�����϶*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireInterval;

	float TowerBarrelPitch;

	/**����ʱ��*/
	FTimerHandle ReloadTimer;

public:
	ATDTowerMissle();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	/**����װ��*/
	virtual void Reload();

	virtual void FireLoop();

	virtual void Fire();

	FTransform GetNearestEnemy();

	virtual void Destroyed()override;

	virtual void OnInjured()override;
};
