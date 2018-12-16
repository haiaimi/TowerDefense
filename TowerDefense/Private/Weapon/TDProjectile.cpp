// Fill out your copyright notice in the Description page of Project Settings.

#include "TDProjectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>


// Sets default values
ATDProjectile::ATDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));
	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	BoomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BoomSprite"));

	ProjectileComponent->UpdatedComponent = ProjectileSprite;
	ProjectileComponent->InitialSpeed = 100.f;
	ProjectileComponent->MaxSpeed = 100.f;
	ProjectileComponent->ProjectileGravityScale = 0.f;
	RootComponent = ProjectileSprite;
}

// Called when the game starts or when spawned
void ATDProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FBoxSphereBounds Bounds = ProjectileSprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	ProjectileCollision->SetBoxExtent(Bounds.BoxExtent);
}

// Called every frame
void ATDProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

