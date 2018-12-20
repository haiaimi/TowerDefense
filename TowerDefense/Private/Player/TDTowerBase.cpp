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


// Sets default values
ATDTowerBase::ATDTowerBase() :
	Health(200.f),
	TowerType(ETowerType::EBase),
	InMapIndex(0)
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

	if (Health < 0)
	{
		ATDTowerBase* Tmp = GetWorld()->SpawnActorDeferred<ATDTowerBase>(ATDTowerBase::StaticClass(), FTransform(GetActorRotation(), GetActorLocation()), GetOwner());
		//ATDTowerBase* Tmp = GetWorld()->SpawnActor<ATDTowerBase>(GetActorLocation(), GetActorRotation());
		if (Tmp)
		{
			Tmp->InMapIndex = InMapIndex;
			UGameplayStatics::FinishSpawningActor(Tmp, FTransform(GetActorRotation(), GetActorLocation()));
		}
		FTransform ExplosionTransform = GetActorTransform();
		ExplosionTransform.SetScale3D(FVector(2.f, 2.f, 2.f));
		if (TowerExpolsionEffect)GetWorld()->SpawnActor<AExplosionEffect>(TowerExpolsionEffect, ExplosionTransform);
		Destroy();
	}

	return 0.f;
}

