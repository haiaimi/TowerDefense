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
	/**±¬Õ¨Sprite*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* BoomSprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UPaperSprite*> Sprites;

	FTimerHandle AlphaTimer;

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
