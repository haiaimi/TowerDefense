// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMap.h"
#include <../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperTileMapComponent.h>
#include <Components/SplineComponent.h>
#include "TDEnemy.h"
#include <Engine/Engine.h>


// Sets default values
ATDMap::ATDMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Map = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	RouteLine = CreateDefaultSubobject<USplineComponent>(TEXT("RouteLine"));
}

// Called when the game starts or when spawned
void ATDMap::BeginPlay()
{
	Super::BeginPlay();

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

