﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDMap.generated.h"

UCLASS()
class TOWERDEFENSE_API ATDMap : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UPaperTileMapComponent* Map;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USplineComponent* RouteLine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ATDEnemy> Enemy_1;
	
public:	
	// Sets default values for this actor's properties
	ATDMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();
	
};
