// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMap.h"
#include <../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperTileMapComponent.h>
#include <Components/SplineComponent.h>
#include "TDEnemy.h"
#include <Engine/Engine.h>
#include "TDTowerBase.h"


// Sets default values
ATDMap::ATDMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Map = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	RouteLine = CreateDefaultSubobject<USplineComponent>(TEXT("RouteLine"));

	Map->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATDMap::BeginPlay()
{
	Super::BeginPlay();

	AllTowerType.Init(ETowerType::EBase, BuildPoints.Num());

	for (int32 i = 0; i < BuildPoints.Num(); ++i)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		ATDTowerBase* Tmp = GetWorld()->SpawnActor<ATDTowerBase>(BaseTower, FTransform(FRotator::ZeroRotator, BuildPoints[i] + FVector(0.f, 10.f, 0.f)), SpawnParameter);
		Tmp->InMapIndex = i;
	}
}

// Called every frame
void ATDMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDMap::SpawnEnemy()
{
	if (Enemy_1)
		GetWorld()->SpawnActor<ATDEnemy>(Enemy_1);
}

void ATDMap::UpdateTowerType(ETowerType::Type InType, int32 Index)
{
	AllTowerType[Index] = InType;
}

