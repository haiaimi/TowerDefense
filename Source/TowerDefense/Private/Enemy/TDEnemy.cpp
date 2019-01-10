// Fill out your copyright notice in the Description page of Project Settings.

#include "TDEnemy.h"
#include "TDController.h"
#include <Engine/World.h>
#include <Components/SplineComponent.h>
#include "TDMap.h"
#include <PaperSpriteComponent.h>
#include <Engine/Engine.h>
#include <Components/BoxComponent.h>
#include "TDTypes.h"
#include "ExplosionEffect.h"
#include "HAIAIMIHelper.h"


// Sets default values
ATDEnemy::ATDEnemy() :
	DestMap(nullptr),
	Speed(50.f),
	Distance(0.f),
	CurType(EEnemyType::EBot),     //默认是机器人类型
	MoveOffset(0.f),
	Health(100.f),
	Bonus(100.f),
	TraceScale(FVector(0.6f, 0.6f, 0.6f)),
	OverlapEnemy(nullptr),
	OffsetChanged(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	EnemySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemySprite"));
	EnemyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyCollision"));
	RootComponent = EnemySprite;
	EnemySprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemySprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	EnemyCollision->SetupAttachment(EnemySprite);
	EnemyCollision->SetCollisionObjectType(COLLISION_ENEMY);
	EnemyCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EnemyCollision->SetCollisionResponseToChannel(COLLISION_MISSLE, ECollisionResponse::ECR_Block);
	EnemyCollision->SetCollisionResponseToChannel(COLLISION_DETECTBOX, ECollisionResponse::ECR_Overlap);
	EnemySprite->TranslucencySortPriority = 1;
}

// Called when the game starts or when spawned
void ATDEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	FBoxSphereBounds Bounds = EnemySprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	Bounds.BoxExtent.Y += 200.f;
	EnemyCollision->SetBoxExtent(Bounds.BoxExtent);

	if (CurType == EEnemyType::EBot)
		MoveOffset = FMath::Rand() % 100 - 50;
}

// Called every frame
void ATDEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DestMap)
	{
		USplineComponent* Spline = DestMap->RouteLine;    //获取地图得路线
		//初始化敌人的位置
		FTransform NewPoint = Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		const FVector rightDir = FRotationMatrix(NewPoint.Rotator()).GetUnitAxis(EAxis::Z);
		SetActorLocationAndRotation(NewPoint.GetLocation() + rightDir * MoveOffset, NewPoint.GetRotation());
	}
	else
	{
		if (GetWorld())
			DestMap = Cast<ATDController>(GetWorld()->GetFirstPlayerController())->CurMap;
	}

	Distance += Speed * DeltaTime;

	if (OverlapEnemy && FMath::Abs(MoveOffset) < 60.f)
	{
		MoveOffset += DeltaTime * OffsetChanged;
		OverlapEnemy->OffsetChanged = -OffsetChanged;
	}
}

float ATDEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		FTransform TraceTransform = GetActorTransform();
		TraceTransform.SetScale3D(TraceScale);
		TraceTransform.SetRotation(FQuat(FRotator(FMath::Rand() % 360, 0.f, 0.f)));
		GetWorld()->SpawnActor<AExplosionEffect>(DeathTrace, TraceTransform);
		if (ATDController* CurController = Cast<ATDController>(GetWorld()->GetFirstPlayerController()))
		{
			CurController->AddScore(Bonus * 2);
			CurController->AddMoney(Bonus);
		}
		Destroy();
	}

	return 0.f;
}

void ATDEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ATDEnemy* Enemy = Cast<ATDEnemy>(OtherActor))
	{
		OverlapEnemy = Enemy;
		const FVector Dir = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const FVector LDir = (FRotationMatrix(GetActorRotation()).GetUnitAxis(EAxis::Z));
		if (FVector::DotProduct(Dir, LDir) > 0.f)
			OffsetChanged = -20.f;
		else 
			OffsetChanged = 20.f;
	}

	Super::NotifyActorBeginOverlap(OtherActor);
}

void ATDEnemy::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (Cast<ATDEnemy>(OtherActor) == OverlapEnemy)
	{
		OverlapEnemy = nullptr;
		OffsetChanged = 0.f;
		Super::NotifyActorEndOverlap(OtherActor);
	}
}

