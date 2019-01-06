// Fill out your copyright notice in the Description page of Project Settings.

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
	TArray<TSubclassOf<class ATDEnemy>> EnemyTypes;

	/**可以建造的点*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<FVector> BuildPoints;

	/**炮台地基*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ATDTowerBase> BaseTower;

	/**用于检测敌人出去*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* OutDetection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AExplosionEffect> ExplosionEffect;

	/**所有炮台的类型*/
	TArray<ETowerType::Type> AllTowerType;

	FTimerHandle SpawnEnemyTimer;
	
public:	
	ATDMap();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemy(int32 Index);
	
	void UpdateTowerType(ETowerType::Type InType, int32 Index);

	/**对*/
	UFUNCTION(BlueprintNativeEvent)
	void ApplyBomb(class UBoxComponent* Box);

	virtual void ApplyBomb_Implementation(class UBoxComponent* Box);

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
