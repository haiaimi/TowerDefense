// Fill out your copyright notice in the Description page of Project Settings.

#include "TDProjectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>
#include "Common/HAIAIMIHelper.h"
#include "TDTypes.h"
#include "TDTowerBase.h"


// Sets default values
ATDProjectile::ATDProjectile():
	Damage(30.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));

	ProjectileSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(COLLISION_ENEMYBULLET);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_ENEMYBULLET, ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_MISSLE, ECollisionResponse::ECR_Ignore);
	
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->UpdatedComponent = ProjectileCollision;
	ProjectileComponent->InitialSpeed = 300.f;
	ProjectileComponent->MaxSpeed = 300.f;
	ProjectileComponent->Velocity = FVector(1.f, 0.f, 0.f);
	ProjectileComponent->ProjectileGravityScale = 0.f;
	
	RootComponent = ProjectileCollision;
	ProjectileSprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATDProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10.f);  //设置子弹的声明周期
	ProjectileSprite->SetRelativeLocation(FVector::ZeroVector);
	FBoxSphereBounds Bounds = ProjectileSprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	Bounds.BoxExtent.Y += 200.f;
	ProjectileCollision->SetBoxExtent(Bounds.BoxExtent);
}

void ATDProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileComponent->OnProjectileStop.AddDynamic(this, &ATDProjectile::OnImpact);
	if (GetOwner())
		ProjectileCollision->MoveIgnoreActors.Add(GetOwner());
}

// Called every frame
void ATDProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDProjectile::Launch(FVector Veolcity)
{
	ProjectileComponent->Velocity = Veolcity;
}

void ATDProjectile::OnImpact(const FHitResult& result)
{
	//爆炸效果
	FVector ImpactPoint = result.ImpactPoint;

	if (ATDTowerBase* Tower = Cast<ATDTowerBase>(result.GetActor()))
	{
		Tower->TakeDamage(Damage, FDamageEvent(), NULL, NULL);
	}
	
	Destroy();
}

void ATDProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

