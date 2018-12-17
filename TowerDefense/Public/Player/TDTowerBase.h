// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDTowerBase.generated.h"

UCLASS()
class TOWERDEFENSE_API ATDTowerBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* TowerSprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* TowerCollision;

public:	
	// Sets default values for this actor's properties
	ATDTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
