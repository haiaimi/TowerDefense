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
	/**�ڹ�Sprites*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPaperSpriteComponent* TankBarrel;
	
	/**̹�˿���Ļ���*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPaperSpriteComponent* TankFire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapon")
	TSubclassOf<class ATDProjectile> TankProjectile;

	/**�������ʱ��*/
	FTimerHandle FireTime;

	/**�����϶*/
	FTimerHandle FireInterval;

	int32 TowerIndex;

	float TankPitch;

public:
	ATDEnemy_Tank();

	virtual void BeginPlay()override;

	virtual void PostInitializeComponents()override;

	virtual void Tick(float DeltaTime)override;

	/**̹�˿���*/
	void Fire();

	/**����ѭ����2���ڷ���һ��*/
	void FireLoop();

	/**��ȡ������̨λ��*/
	FVector GetNearestTower();
};
