// Fill out your copyright notice in the Description page of Project Settings.

#include "TDEnemy_Tank.h"
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>
#include <Engine/Engine.h>
#include <TimerManager.h>
#include "Weapon/TDProjectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>



ATDEnemy_Tank::ATDEnemy_Tank()
{
	TankBarrel = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankBarrel"));
	TankFire = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankFire"));

	if (TankFire&&TankBarrel)TankFire->SetupAttachment(TankBarrel);
}

void ATDEnemy_Tank::BeginPlay()
{
	Super::BeginPlay();

	FireLoop();
}

void ATDEnemy_Tank::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FTransform FireTransform = TankBarrel->GetSocketTransform(TEXT("FireSocket"));
	TankBarrel->SetRelativeLocation(FVector(0.f, 10.f, 0.f));
	TankFire->SetRelativeLocation(FireTransform.GetLocation() - TankBarrel->GetComponentLocation() + FVector(-10.f, 5.f, 0.f));
	TankFire->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	TankFire->SetVisibility(false);
}

void ATDEnemy_Tank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*FVector vec;
	FRotator(vec);*/
	//Fire();

	/*GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TankBarrel->GetComponentLocation().ToString());
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, GetActorLocation().ToString());*/
}

void ATDEnemy_Tank::Fire()
{
	FTransform FireTransform = TankBarrel->GetSocketTransform(TEXT("FireSocket"));
	ATDProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<ATDProjectile>(TankProjectile, FireTransform);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->ProjectileComponent->Velocity = FireTransform.Rotator().Vector()*200.f;
		UGameplayStatics::FinishSpawningActor(SpawnedProjectile, FireTransform);
	}

	TankFire->SetVisibility(true);
	FTimerDelegate Delegate;
	Delegate.BindLambda([&]() {
		TankFire->SetVisibility(false);
		});

	GetWorld()->GetTimerManager().SetTimer(FireTime, Delegate, 0.5f, false);
}

void ATDEnemy_Tank::FireLoop()
{
	GetWorld()->GetTimerManager().SetTimer(FireInterval, this, &ATDEnemy_Tank::FireLoop, 2.f, false);

	Fire();
}
