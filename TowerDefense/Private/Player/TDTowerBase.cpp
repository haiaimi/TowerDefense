// Fill out your copyright notice in the Description page of Project Settings.

#include "TDTowerBase.h"
#include <Components/BoxComponent.h>
#include <PaperSpriteComponent.h>
#include "TDTypes.h"
#include <ConstructorHelpers.h>
#include <PaperSprite.h>
#include <Engine/World.h>
#include "ExplosionEffect.h"
#include "TDMap.h"
#include <Kismet/GameplayStatics.h>
#include "TDController.h"
#include <Engine/Engine.h>
#include "../UI/Widgets/SRepairWidget.h"
#include "Common/HAIAIMIHelper.h"
#include <Engine/GameViewportClient.h>


// Sets default values
ATDTowerBase::ATDTowerBase() :
	MaxHealth(200.f),
	Health(200.f),
	TowerType(ETowerType::EBase),
	InMapIndex(0),
	BuildCost(400),
	RepairWidget(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPaperSprite> SpriteFinder(TEXT("/Game/Weapon/Sprite/towerDefense_tile018_Sprite"));

	TowerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TowerCollision"));
	TowerSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TowerSprite"));

	if (SpriteFinder.Succeeded())TowerSprite->SetSprite(SpriteFinder.Object);

	RootComponent = TowerSprite;
	TowerCollision->SetupAttachment(TowerSprite);

	TowerSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TowerSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TowerCollision->SetCollisionObjectType(COLLISION_TOWERBASE);
	TowerCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TowerCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	TowerSprite->TranslucencySortPriority = 2;
}

// Called when the game starts or when spawned
void ATDTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ATDMap* OwnerMap = Cast<ATDMap>(GetOwner()))
		OwnerMap->UpdateTowerType(TowerType, InMapIndex); //重新设置炮台类型

	FBoxSphereBounds Bounds = TowerSprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	Bounds.BoxExtent.Y += 200.f;
	TowerCollision->SetBoxExtent(Bounds.BoxExtent);
}

// Called every frame
void ATDTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ATDTowerBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health / MaxHealth < 1.f && GetWorld())
	{
		ATDController* CurController = Cast<ATDController>(GetWorld()->GetFirstPlayerController());
		if (CurController)
		{
			FVector2D ScreenLocation;
			CurController->ProjectWorldLocationToScreen(GetActorLocation() + FVector(50.f, 0.f, 120.f), ScreenLocation);

			if (!RepairWidget.IsValid() && GEngine)
			{
				OnInjured();
				SAssignNew(RepairWidget, SRepairWidget)
				.SpawnPos(HAIAIMIHelper::ConvertToNormalCoord(ScreenLocation));

				GEngine->GameViewport->AddViewportWidgetContent(
					SNew(SWeakWidget)
					.PossiblyNullContent(RepairWidget.ToSharedRef()),
					0
				);
			}
		}
	}

	if (Health < 0)
	{
		ATDTowerBase* Tmp = GetWorld()->SpawnActorDeferred<ATDTowerBase>(ATDTowerBase::StaticClass(), FTransform(GetActorRotation(), GetActorLocation()), GetOwner());
		if (Tmp)
		{
			Tmp->InMapIndex = InMapIndex;
			UGameplayStatics::FinishSpawningActor(Tmp, FTransform(GetActorRotation(), GetActorLocation()));
		}
		FTransform ExplosionTransform = GetActorTransform();
		if (TowerExpolsionEffect)
		{
			AExplosionEffect* ExplosionEffect = GetWorld()->SpawnActor<AExplosionEffect>(TowerExpolsionEffect, ExplosionTransform);
			ExplosionEffect->SetActorScale3D(FVector(2.f, 2.f, 2.f));
		}
		Destroy();
	}

	return 0.f;
}

