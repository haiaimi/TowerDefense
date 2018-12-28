// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionEffect.generated.h"

UCLASS()
class TOWERDEFENSE_API AExplosionEffect : public AActor
{
	GENERATED_BODY()
	
public:
	/**爆炸Sprite*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* BoomSprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UPaperSprite*> Sprites;

	FTimerHandle AlphaTimer;

	/**每帧时间*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PerFrameTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector MoveDir;

	/**缩放参数：X分量为初始尺寸，Y为尺寸增加速度*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D ScaleParam;

	int CurSpriteIndex;

public:	
	// Sets default values for this actor's properties
	AExplosionEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateSprites();
	
};
