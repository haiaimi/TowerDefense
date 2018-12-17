// Fill out your copyright notice in the Description page of Project Settings.

#include "TDProjectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>
#include "Common/HAIAIMIHelper.h"


// Sets default values
ATDProjectile::ATDProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollision"));
	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));
	BoomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BoomSprite"));

	ProjectileSprite->SetSimulatePhysics(true);
	ProjectileSprite->SetEnableGravity(false);
	ProjectileSprite->SetConstraintMode(EDOFMode::XZPlane);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileComponent->UpdatedComponent = ProjectileSprite;
	ProjectileComponent->InitialSpeed = 300.f;
	ProjectileComponent->MaxSpeed = 300.f;
	ProjectileComponent->ProjectileGravityScale = 0.5f;
	RootComponent = ProjectileSprite;
	ProjectileCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATDProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision->SetRelativeLocation(FVector::ZeroVector);
	//ProjectileComponent->Velocity = FVector(0.f, 0.f, 400.f);
}

void ATDProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FBoxSphereBounds Bounds = ProjectileSprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	ProjectileCollision->SetBoxExtent(Bounds.BoxExtent);

	ProjectileComponent->OnProjectileStop.AddDynamic(this, &ATDProjectile::OnImpact);

	//RootComponent = ProjectileCollision;
	//ProjectileSprite->SetupAttachment(RootComponent);
	//ProjectileCollision->SetRelativeLocation(FVector::ZeroVector);
}

// Called every frame
void ATDProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//HAIAIMIHelper::Debug_ScreenMessage(ProjectileCollision->GetComponentLocation().ToString());
	//HAIAIMIHelper::Debug_ScreenMessage(ProjectileSprite->GetComponentLocation().ToString());
}

void ATDProjectile::OnImpact(const FHitResult& result)
{
	HAIAIMIHelper::Debug_ScreenMessage(TEXT("Stop Stop Stop"));
	
	Destroy();
}

