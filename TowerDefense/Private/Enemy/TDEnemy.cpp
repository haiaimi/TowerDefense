// Fill out your copyright notice in the Description page of Project Settings.

#include "TDEnemy.h"
#include "TDController.h"
#include <Engine/World.h>
#include <Components/SplineComponent.h>
#include "TDMap.h"
#include <PaperSpriteComponent.h>
#include <Engine/Engine.h>
#include <Components/BoxComponent.h>


// Sets default values
ATDEnemy::ATDEnemy(): 
	DestMap(nullptr),
	Speed(50.f),
	Distance(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	EnemySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemySprite"));
	EnemyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyCollision"));
	RootComponent = EnemySprite;
	EnemyCollision->SetupAttachment(EnemySprite);
}

// Called when the game starts or when spawned
void ATDEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	FBoxSphereBounds Bounds = EnemySprite->CalcBounds(FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	Bounds.BoxExtent.Y += 50.f;
	EnemyCollision->SetBoxExtent(Bounds.BoxExtent);
}

// Called every frame
void ATDEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("Update Pos"));
	if (DestMap)
	{
		USplineComponent* Spline = DestMap->RouteLine;    //获取地图得路线
		//初始化敌人得位置
		FTransform NewPoint = Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		SetActorLocationAndRotation(NewPoint.GetLocation(), NewPoint.GetRotation());
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Black, TEXT("Update Pos"));
	}
	else
	{
		if (GetWorld())
			DestMap = Cast<ATDController>(GetWorld()->GetFirstPlayerController())->CurMap;
	}

	Distance += Speed * DeltaTime;
}

