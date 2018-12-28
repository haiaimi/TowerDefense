// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDProjectile.generated.h"

UCLASS()
class TOWERDEFENSE_API ATDProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* ProjectileSprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* ProjectileCollision;

	/**爆炸效果*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AExplosionEffect> ExplosionEffect;

	/**子弹伤害*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damage;
	
public:	
	// Sets default values for this actor's properties
	ATDProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents()override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**子弹发射*/
	virtual void Launch(FVector Veolcity);

	UFUNCTION()
	virtual void OnImpact(const FHitResult& result);
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;
};
