﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "TDEnemy_Tank.h"
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>
#include <Engine/Engine.h>
#include <TimerManager.h>
#include "Weapon/TDProjectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Common/HAIAIMIHelper.h"
#include "TDMap.h"
#include "TDTypes.h"
#include <Sound/SoundCue.h>

ATDEnemy_Tank::ATDEnemy_Tank() :
	TowerIndex(0),
	TankPitch(0.f),
	BulletDamage(100.f)
{
	TankBarrel = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankBarrel"));
	TankFire = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankFire"));

	TankFire->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TankBarrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TankBarrel->SetupAttachment(RootComponent);
	TankBarrel->TranslucencySortPriority = 2;

	if (TankFire && TankBarrel)TankFire->SetupAttachment(TankBarrel);
	CurType = EEnemyType::ETank;
}

void ATDEnemy_Tank::BeginPlay()
{
	Super::BeginPlay();

	FireLoop();
}

void ATDEnemy_Tank::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TankBarrel->SetRelativeLocation(FVector(0.f, 10.f, 0.f));
	TankFire->SetRelativeLocation(FVector(35.f, 5.f, 3.f));
	TankFire->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	TankFire->SetVisibility(false);
}

void ATDEnemy_Tank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector NearPoint = GetNearestTower();
	const FVector Dir = (NearPoint - TankBarrel->GetComponentLocation()).GetSafeNormal();
	const FVector LDir = FRotationMatrix(TankBarrel->GetComponentRotation()).GetUnitAxis(EAxis::Z);
	const float DotRes = FVector::DotProduct(Dir, LDir);
	
	TankPitch += (DotRes > 0 ? 40.f : -40.f)*DeltaTime;
	TankBarrel->SetWorldRotation(FRotator(TankPitch, 0.f, 0.f));
}

void ATDEnemy_Tank::Fire()
{
	FTransform FireTransform = TankBarrel->GetSocketTransform(TEXT("FireSocket"));
	FTransform SpawnTransform = TankFire->GetSocketTransform(TEXT("BulletSocket"));
	ATDProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(TankProjectile, SpawnTransform, this);
	if (SpawnedProjectile)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		SpawnedProjectile->Launch(TankBarrel->GetComponentRotation().Vector()*300.f);
		SpawnedProjectile->Damage = BulletDamage;
		UGameplayStatics::FinishSpawningActor(SpawnedProjectile, FTransform(FRotator::ZeroRotator, SpawnTransform.GetLocation()));
	}

	TankFire->SetVisibility(true);
	FTimerDelegate Delegate;
	Delegate.BindLambda([&]() {
		TankFire->SetVisibility(false);
		});

	GetWorld()->GetTimerManager().SetTimer(FireTime, Delegate, 0.1f, false);
}

void ATDEnemy_Tank::FireLoop()
{
	GetWorld()->GetTimerManager().SetTimer(FireInterval, this, &ATDEnemy_Tank::FireLoop, 2.f, false);

	if (GetNearestTower() != FVector(0.f, 0.f, 0.f))
		Fire();
}

FVector ATDEnemy_Tank::GetNearestTower()
{
	FVector ResultPoint(0.f, 0.f, 0.f);
	if (DestMap)
	{
		TArray<FVector>& Points = DestMap->BuildPoints;
		TArray<ETowerType::Type>& AllTowerType = DestMap->AllTowerType;
		float MinDistance = INT_MAX;
		for (int32 i = 0; i < Points.Num(); ++i)
		{
			if (AllTowerType[i] == ETowerType::EBase)continue;
			
			float TmpDistance = (Points[i] - GetActorLocation()).Size();
			if (TmpDistance < MinDistance)
			{
				ResultPoint = Points[i];
				MinDistance = TmpDistance;
			}
		}

	} 

	return ResultPoint;
}
