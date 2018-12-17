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

	/**±¬Õ¨Sprite*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* BoomSprite;
	
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

	/**×Óµ¯·¢Éä*/
	virtual void Launch(FVector Veolcity);

	virtual void OnImpact(const FHitResult& result);
	
};
