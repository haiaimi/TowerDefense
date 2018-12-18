// Fill out your copyright notice in the Description page of Project Settings.

#include "TDTower2Missle.h"
#include <PaperSpriteComponent.h>
#include "Missle.h"
#include <Engine/World.h>
#include "TDEnemy.h"
#include <EngineUtils.h>
#include "Common/HAIAIMIHelper.h"
#include <TimerManager.h>
#include <Components/BoxComponent.h>


ATDTower2Missle::ATDTower2Missle() :
	FireInterval(1.f),
	TowerBarrelPitch(0.f)
{
	TowerBarrel = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TowerBarrel"));
	TowerBarrel->SetupAttachment(TowerSprite);
	TowerType = ETowerType::ETower2Missle;
	TowerBarrel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TowerCollision->SetCollisionObjectType(COLLISION_TOWER);
	TowerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	TowerCollision->SetCollisionResponseToChannel(COLLISION_MISSLE, ECollisionResponse::ECR_Ignore);
	TowerCollision->SetCollisionResponseToChannel(COLLISION_ENEMYBULLET, ECollisionResponse::ECR_Block);
}

void ATDTower2Missle::BeginPlay()
{
	Super::BeginPlay();

	Missles.Reset(0);
	Reload();
}

void ATDTower2Missle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FTransform NearPoint = GetNearestEnemy();
	FVector Dir = (NearPoint.GetLocation() - GetActorLocation()).GetSafeNormal();
	FVector LDir = -FRotationMatrix(TowerBarrel->GetComponentRotation()).GetUnitAxis(EAxis::X);
	Dir.Y = 0.f;
	LDir.Y = 0.f;
	const float DotRes = FVector::DotProduct(Dir, LDir);
	const float EnemySubAngle = FVector::DotProduct(-Dir, NearPoint.Rotator().Vector());

	TowerBarrelPitch += (DotRes > 0 ? 50.f : -50.f)*DeltaTime;
	TowerBarrel->SetWorldRotation(FRotator(TowerBarrelPitch, 0.f, 0.f));
	//HAIAIMIHelper::Debug_ScreenMessage(FString::SanitizeFloat(TowerBarrelPitch));
	if (FireInterval <= 0.f)
	{
		FVector FDir = FRotationMatrix(TowerBarrel->GetComponentRotation()).GetUnitAxis(EAxis::Z);
		FDir.Y = 0.f;
		const float SubDegree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Dir, FDir)));
		if (SubDegree < 10.f)
			Fire();
	}
	FireInterval -= DeltaTime;
}

void ATDTower2Missle::Reload()
{
	int32 MissleIndex = 1;
	FString SocketName = FString("Missle") + FString::FormatAsNumber(MissleIndex);
	while (TowerBarrel->DoesSocketExist(*SocketName))
	{
		HAIAIMIHelper::Debug_ScreenMessage(TEXT("Have Spawned Missle"));
		AMissle* TmpMissle = GetWorld()->SpawnActor<AMissle>(MissleType, TowerBarrel->GetSocketTransform(*SocketName));
		TmpMissle->AttachToComponent(TowerBarrel, FAttachmentTransformRules::KeepWorldTransform, *SocketName);
		if (TmpMissle)Missles.Add(TmpMissle);
		SocketName = FString("Missle") + FString::FormatAsNumber(++MissleIndex);
	}
}

void ATDTower2Missle::FireLoop()
{

}

void ATDTower2Missle::Fire()
{
	if (Missles.Num() == 0)return;

	AMissle* LaunchMissle = Missles.Pop();
	LaunchMissle->Launch(FRotationMatrix(LaunchMissle->GetActorRotation()).GetUnitAxis(EAxis::Z)*500.f);
	FireInterval = 1.f;

	if (Missles.Num() == 0)
	{
		FTimerDelegate Delegate;
		Delegate.BindLambda([&]() {
			Reload();
			FireInterval = 0.5f;
			});

		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, Delegate, 2.f, false);
	}
}

FTransform ATDTower2Missle::GetNearestEnemy()
{
	FTransform Res;
	float MinDistance = INT_MAX;
	for (TActorIterator<ATDEnemy> iter(GetWorld()); iter; ++iter)
	{
		float Tmp = ((*iter)->GetActorLocation() - GetActorLocation()).Size();
		if (Tmp < MinDistance)
		{
			Res = (*iter)->GetActorTransform();
			MinDistance = Tmp;
		}
	}

	return Res;
}
