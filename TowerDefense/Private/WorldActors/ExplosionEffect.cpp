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
	CurSpriteIndex(0),
	PerFrameTime(0.16f),
	MoveDir(0.f,0.f,0.f),
	ScaleParam(1.f,1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	BoomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BoomSprite"));
	BoomSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoomSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoomSprite->SetCollisionObjectType(COLLISION_EXPLOSION);
	BoomSprite->TranslucencySortPriority = 6.f;
	RootComponent = BoomSprite;
}

// Called when the game starts or when spawned
void AExplosionEffect::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.f, 1.f, 1.f)*ScaleParam.X);

	UpdateSprites();
}

// Called every frame
void AExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector AddedScale(ScaleParam.Y*DeltaTime, 1.f, ScaleParam.Y*DeltaTime);
	BoomSprite->SetWorldScale3D(BoomSprite->GetComponentScale() + AddedScale);
	SetActorLocation(GetActorLocation() + MoveDir * DeltaTime*60.f);
	SetActorScale3D(GetActorScale() + AddedScale);
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

	GetWorld()->GetTimerManager().SetTimer(AlphaTimer, this, &AExplosionEffect::UpdateSprites, PerFrameTime);
}

