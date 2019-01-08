// Fill out your copyright notice in the Description page of Project Settings.

#include "TDTowerGun.h"
#include <PaperSpriteComponent.h>
#include <Engine/World.h>
#include "TDProjectile.h"
#include "HAIAIMIHelper.h"
#include <PaperSprite.h>
#include <ConstructorHelpers.h>
#include <TimerManager.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

ATDTowerGun::ATDTowerGun()
{
	SocketNames.Reset(0);

	TowerBarrel->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
	static ConstructorHelpers::FObjectFinder<UPaperSprite> FireS1(TEXT("/Game/Towers/Sprites/towerDefense_tile295_Sprite"));
	FireS = FireS1.Object;
}

void ATDTowerGun::BeginPlay()
{
	int32 FirePoint = 1;
	FString SocketName = FString(TEXT("FireSocket")) + FString::FormatAsNumber(FirePoint);
	while(TowerBarrel->DoesSocketExist(*SocketName))
	{
		SocketNames.Add(SocketName);
		SocketName = FString(TEXT("FireSocket")) + FString::FormatAsNumber(++FirePoint);
		FireSprites.Add(NewObject<UPaperSpriteComponent>(this));
	}

	for (int32 i = 0; i < FireSprites.Num(); ++i)
	{
		FireSprites[i]->SetSprite(FireS);
		FireSprites[i]->AttachToComponent(TowerBarrel, FAttachmentTransformRules::KeepRelativeTransform, *SocketNames[i]);
		FireSprites[i]->TranslucencySortPriority = 6.f;
		FireSprites[i]->RegisterComponentWithWorld(GetWorld());   //一定要注册一下，否则不会显示
		FireSprites[i]->RegisterComponent();
		FireSprites[i]->SetVisibility(false);
		FireSprites[i]->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));
	}

	Super::BeginPlay();
}

void ATDTowerGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDTowerGun::Fire()
{
	for (int32 i = 0; i < SocketNames.Num(); ++i)
	{
		FTransform FireTransform = TowerBarrel->GetSocketTransform(*SocketNames[i]);
		ATDProjectile* Temp = nullptr;
		if (GetWorld())
			Temp = GetWorld()->SpawnActor<ATDProjectile>(WeaponType, FireTransform);
		if (Temp)
		{
			Temp->Launch(FRotationMatrix(FireTransform.Rotator()).GetUnitAxis(EAxis::Z)*500.f);
			Temp->SetActorScale3D(FVector(0.6f, 0.6f, 0.6f));
		}
		FireSprites[i]->SetVisibility(true);
	}

	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	FireInterval = 2.f;
	FTimerDelegate FireDelegate;
	FireDelegate.BindLambda([&]() {
		for (auto& iter : FireSprites)
			iter->SetVisibility(false);
		});

	GetWorldTimerManager().SetTimer(ReloadTimer, FireDelegate, 0.3f, false);
}
