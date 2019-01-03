// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TDTowerMissle.h"
#include "TDTowerGun.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENSE_API ATDTowerGun : public ATDTowerMissle
{
	GENERATED_BODY()
	
public:
	ATDTowerGun();

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	/**ÖØÐÂ×°µ¯*/
	void Reload() override {};

	virtual void Fire()override;
	
public:
	TArray<class UPaperSpriteComponent*> FireSprites;

	class UPaperSprite* FireS;

private:
	TArray<FString> SocketNames;
};
