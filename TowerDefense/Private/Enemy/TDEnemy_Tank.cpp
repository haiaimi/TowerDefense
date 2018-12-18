// Fill out your copyright notice in the Description page of Project Settings.

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



ATDEnemy_Tank::ATDEnemy_Tank() :
	TowerIndex(0),
	TankPitch(0.f)
{
	TankBarrel = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankBarrel"));
	TankFire = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankFire"));

	TankFire->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TankBarrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TankBarrel->SetupAttachment(RootComponent);
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
	
	//HAIAIMIHelper::Debug_ScreenMessage(FString::Printf(TEXT("CurPitch: %.4f, NextPitch: %.4f"), TankBarrel->GetComponentRotation().Pitch, TankBarrel->GetComponentRotation().Pitch + (DotRes > 0 ? 40.f : -40.f)*DeltaTime));
	TankPitch += (DotRes > 0 ? 40.f : -40.f)*DeltaTime;
	TankBarrel->SetWorldRotation(FRotator(HAIAIMIHelper::AdaptAngle(TankPitch), 0.f, 0.f));
}

void ATDEnemy_Tank::Fire()
{
	FTransform FireTransform = TankBarrel->GetSocketTransform(TEXT("FireSocket"));
	FTransform SpawnTransform = TankFire->GetSocketTransform(TEXT("BulletSocket"));
	ATDProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(TankProjectile, SpawnTransform, this);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->Launch(TankBarrel->GetComponentRotation().Vector()*300.f);
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

	Fire();
}

FVector ATDEnemy_Tank::GetNearestTower()
{
	FVector ResultPoint;
	if (DestMap)
	{
		TArray<FVector>& Points = DestMap->BuildPoints;
		if (TowerIndex + 1 >= Points.Num())
			return Points[TowerIndex];
		else
		{
			const float TankToCur = (Points[TowerIndex] - GetActorLocation()).Size();
			const float TankToNext = (Points[TowerIndex + 1] - GetActorLocation()).Size();

			if (TankToCur <= TankToNext)
				return Points[TowerIndex];
			else
				return Points[++TowerIndex];
		}
	} 

	return ResultPoint;
}
