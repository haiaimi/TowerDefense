// Fill out your copyright notice in the Description page of Project Settings.

#include "Missle.h"
#include <PaperSpriteComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/BoxComponent.h>
#include "TDTypes.h"
#include "ExplosionEffect.h"
#include <Engine/World.h>
#include "Common/HAIAIMIHelper.h"


AMissle::AMissle()
{
	BulletFire = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletFire"));
	BulletFire->SetVisibility(false);
	ProjectileComponent->InitialSpeed = 0.f;   
	ProjectileComponent->MaxSpeed = 600.f;
	ProjectileComponent->Velocity = FVector(0.f, 0.f, 1.f);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(COLLISION_MISSLE);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_TOWER, ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_EXPLOSION, ECollisionResponse::ECR_Ignore);
}

void AMissle::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMissle::BeginPlay()
{
	Super::BeginPlay();

	BulletFire->AttachToComponent(ProjectileSprite, FAttachmentTransformRules::KeepRelativeTransform, TEXT("FireSocket"));
	BulletFire->SetRelativeRotation(FRotator(180.f, 0.f, 0.f));
}

void AMissle::Launch(FVector Veolcity)
{
	//Super::Launch(Veolcity);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ProjectileComponent->Velocity = Veolcity;
	BulletFire->SetVisibility(true);
}

void AMissle::OnImpact(const FHitResult& result)
{
	FVector ImpatcPoint = result.ImpactPoint;
	ImpatcPoint.Y = 10.f;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	AExplosionEffect* Tmp = GetWorld()->SpawnActor<class AExplosionEffect>(ExplosionEffect, FTransform(FRotator::ZeroRotator, result.ImpactPoint + FVector(0.f, 20.f, 0.f)), SpawnParameters);

	HAIAIMIHelper::Debug_ScreenMessage(TEXT("Have Spawned Explosion Effect"));

	Destroy();
}
