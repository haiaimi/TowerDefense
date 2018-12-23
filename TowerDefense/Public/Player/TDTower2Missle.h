// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TDTowerBase.h"
#include "TDTower2Missle.generated.h"

/**
 *  �����������������̨
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

	/**�����*/
	class TArray<class AMissle*> Missles;

	/**�����϶*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireInterval;

	float TowerBarrelPitch;

	/**����ʱ��*/
	FTimerHandle ReloadTimer;

public:
	ATDTower2Missle();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	/**����װ��*/
	void Reload();

	void FireLoop();

	void Fire();

	FTransform GetNearestEnemy();

	virtual void Destroyed()override;

	virtual void OnInjured()override;
};
