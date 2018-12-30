﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDTypes.h"
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

	/**可以建造的点*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<FVector> BuildPoints;

	/**炮台地基*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ATDTowerBase> BaseTower;

	/**用于检测敌人出去*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* OutDetection;

	/**所有炮台的类型*/
	TArray<ETowerType::Type> AllTowerType;
	
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
	
	void UpdateTowerType(ETowerType::Type InType, int32 Index);

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
