// Fill out your copyright notice in the Description page of Project Settings.

#include "TDTowerBase.h"
#include <Components/BoxComponent.h>
#include <PaperSpriteComponent.h>
#include "TDTypes.h"


// Sets default values
ATDTowerBase::ATDTowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TowerCollision"));
	TowerSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TowerSprite"));

	RootComponent = TowerSprite;
	TowerCollision->SetupAttachment(TowerSprite);

	TowerSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TowerCollision->SetCollisionObjectType(COLLISION_TOWERBASE);
	TowerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TowerCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void ATDTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	FBoxSphereBounds Bounds = TowerSprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	Bounds.BoxExtent.Y += 50.f;
	TowerCollision->SetBoxExtent(Bounds.BoxExtent);
}

// Called every frame
void ATDTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

