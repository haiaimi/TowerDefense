// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TDProjectile.h"
#include "Missle.generated.h"

/**
 * 导弹类
 */
UCLASS()
class TOWERDEFENSE_API AMissle : public ATDProjectile
{
	GENERATED_BODY()
	
public:
	/**火箭弹尾焰*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* BulletFire;

public:
	AMissle();

	virtual void PostInitializeComponents()override;

	virtual void BeginPlay()override;

	/**发射火箭弹*/
	virtual void Launch(FVector Veolcity)override;

	virtual void OnImpact(const FHitResult& result)override;
};
