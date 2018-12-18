// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosionEffect.h"
#include <PaperSpriteComponent.h>
#include <PaperSprite.h>
#include <Engine/Texture.h>
#include <TimerManager.h>
#include <Engine/World.h>
#include "TDTypes.h"


// Sets default values
AExplosionEffect::AExplosionEffect():
	CurSpriteIndex(0)
{
	PrimaryActorTick.bCanEverTick = true;

	BoomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BoomSprite"));
	BoomSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoomSprite->SetCollisionObjectType(COLLISION_EXPLOSION);
}

// Called when the game starts or when spawned
void AExplosionEffect::BeginPlay()
{
	Super::BeginPlay();

	UpdateSprites();
}

// Called every frame
void AExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosionEffect::UpdateSprites()
{
	if (CurSpriteIndex == Sprites.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(AlphaTimer);
		Destroy();
	}

	if (CurSpriteIndex < Sprites.Num())
		BoomSprite->SetSprite(Sprites[CurSpriteIndex++]);

	GetWorld()->GetTimerManager().SetTimer(AlphaTimer, this, &AExplosionEffect::UpdateSprites, 0.16f);
}

