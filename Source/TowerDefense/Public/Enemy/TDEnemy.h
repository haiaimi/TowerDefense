// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDTypes.h"
#include "TDEnemy.generated.h"

UCLASS()
class TOWERDEFENSE_API ATDEnemy : public AActor
{
	GENERATED_BODY()

public:
	/**前进速度*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;

	/**敌人在当前路线中前进得距离*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Distance;

	/**敌人Sprites*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* EnemySprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* EnemyCollision;

	/**生命值*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;

	/**击杀奖励金*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Bonus;

	/**敌人死亡效果*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class TSubclassOf<class AExplosionEffect> DeathTrace;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraceScale;

	EEnemyType::Type CurType;

protected:
	/** 目标地图*/
	class ATDMap* DestMap;
	
	/**前进偏移量*/
	float MoveOffset;
public:	
	// Sets default values for this actor's properties
	ATDEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
};
