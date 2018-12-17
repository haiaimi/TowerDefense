// Fill out your copyright notice in the Description page of Project Settings.

#include "TDTower2Missle.h"
#include <PaperSpriteComponent.h>
#include "Missle.h"
#include <Engine/World.h>
#include "TDEnemy.h"
#include <EngineUtils.h>
#include "Common/HAIAIMIHelper.h"


ATDTower2Missle::ATDTower2Missle() :
	FireInterval(1.f),
	TowerBarrelPitch(0.f)
{
	TowerBarrel = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TowerBarrel"));
	TowerBarrel->SetupAttachment(TowerSprite);
}

void ATDTower2Missle::BeginPlay()
{
	Super::BeginPlay();

	Missles.Reset(0);
}

void ATDTower2Missle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector NearPoint = GetNearestEnemy();
	FVector Dir = (NearPoint - GetActorLocation()).GetSafeNormal();
	FVector LDir = -FRotationMatrix(TowerBarrel->GetComponentRotation()).GetUnitAxis(EAxis::X);
	Dir.Y = 0.f;
	LDir.Y = 0.f;
	const float DotRes = FVector::DotProduct(Dir, LDir);

	TowerBarrelPitch += (DotRes > 0 ? 40.f : -40.f)*DeltaTime;
	TowerBarrel->SetWorldRotation(FRotator(HAIAIMIHelper::AdaptAngle(TowerBarrelPitch), 0.f, 0.f));

	if (FireInterval <= 0.f)
	{
		FVector FDir = FRotationMatrix(TowerBarrel->GetComponentRotation()).GetUnitAxis(EAxis::Z);
		FDir.Y = 0.f;
		const float SubDegree = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Dir, FDir)));
		HAIAIMIHelper::Debug_ScreenMessage(FString::SanitizeFloat(SubDegree));
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
		if (TmpMissle)Missles.Add(TmpMissle);
		SocketName = FString("Missle") + FString::FormatAsNumber(++MissleIndex);
	}
}

void ATDTower2Missle::FireLoop()
{

}

void ATDTower2Missle::Fire()
{
	if (Missles.Num() == 0)
	{
		Reload();
		FireInterval = 0.5f;
		return;
	}

	AMissle* LaunchMissle = Missles.Pop();
	LaunchMissle->Launch(FRotationMatrix(LaunchMissle->GetActorRotation()).GetUnitAxis(EAxis::Z)*500.f);
	FireInterval = 1.f;

	if (Missles.Num() == 0)
		FireInterval = 2.f;
}

FVector ATDTower2Missle::GetNearestEnemy()
{
	FVector Res;
	float MinDistance = INT_MAX;
	for (TActorIterator<ATDEnemy> iter(GetWorld()); iter; ++iter)
	{
		float Tmp = ((*iter)->GetActorLocation() - GetActorLocation()).Size();
		if (Tmp < MinDistance)
		{
			Res = (*iter)->GetActorLocation();
			MinDistance = Tmp;
		}
	}

	return Res;
}
