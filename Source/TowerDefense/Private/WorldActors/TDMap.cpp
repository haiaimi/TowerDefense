// Fill out your copyright notice in the Description page of Project Settings.

#include "TDMap.h"
#include <../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperTileMapComponent.h>
#include <Components/SplineComponent.h>
#include "TDEnemy.h"
#include <Engine/Engine.h>
#include "TDTowerBase.h"
#include <Components/BoxComponent.h>
#include "TDController.h"
#include "HAIAIMIHelper.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "ExplosionEffect.h"
#include <Kismet/GameplayStatics.h>
#include "TimerManager.h"


// Sets default values
ATDMap::ATDMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Map = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	RouteLine = CreateDefaultSubobject<USplineComponent>(TEXT("RouteLine"));
	OutDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("OutDetection"));

	OutDetection->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OutDetection->SetCollisionObjectType(COLLISION_DETECTBOX);
	OutDetection->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OutDetection->OnComponentBeginOverlap.AddDynamic(this, &ATDMap::OnBoxBeginOverlap);
	Map->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATDMap::BeginPlay()
{
	Super::BeginPlay();

	AllTowerType.Init(ETowerType::EBase, BuildPoints.Num());

	for (int32 i = 0; i < BuildPoints.Num(); ++i)
	{
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		ATDTowerBase* Tmp = GetWorld()->SpawnActor<ATDTowerBase>(BaseTower, FTransform(FRotator::ZeroRotator, BuildPoints[i] + FVector(0.f, 10.f, 0.f)), SpawnParameter);
		Tmp->InMapIndex = i;
	}

	FTransform BoxTransform = RouteLine->GetTransformAtDistanceAlongSpline(RouteLine->GetSplineLength(), ESplineCoordinateSpace::World);
	OutDetection->SetWorldTransform(BoxTransform);
	OutDetection->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	HAIAIMIHelper::PrepareJson(TEXT("Level1.json"));
	SpawnEnemy(0);
}

// Called every frame
void ATDMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDMap::SpawnEnemy(int32 Index)
{
	if (Index >= HAIAIMIHelper::GetEnemyCounts())return;
	float EnemyType = -1.f, NextTime = -1.f;
	HAIAIMIHelper::GetNumberFromJson(TEXT("Level1"), TEXT("EnemyType"), Index, EnemyType);
	HAIAIMIHelper::GetNumberFromJson(TEXT("Level1"), TEXT("NextTime"), Index, NextTime);
	//HAIAIMIHelper::Debug_ScreenMessage(FString::FormatAsNumber(EnemyType), 5.f);
	GetWorld()->SpawnActor<ATDEnemy>(EnemyTypes[EnemyType]);

	if (NextTime >= 0.f)
	{
		FTimerDelegate SpawnDelegate;
		SpawnDelegate.BindLambda([Index, this]() {
			SpawnEnemy(Index + 1);
			});
		GetWorldTimerManager().SetTimer(SpawnEnemyTimer, SpawnDelegate, NextTime, false);
	}
}

void ATDMap::UpdateTowerType(ETowerType::Type InType, int32 Index)
{
	AllTowerType[Index] = InType;
}

void ATDMap::ApplyBomb_Implementation(class UBoxComponent* Box)
{
	if(Box)
	{
		TArray<AActor*> OverlapActors;
		Box->GetOverlappingActors(OverlapActors, ATDEnemy::StaticClass());
		
		for(auto Iter = OverlapActors.CreateIterator();Iter;++Iter)
		{
			(*Iter)->TakeDamage(50.f, FDamageEvent(), Cast<APlayerController>(GetOwner()), this);
		}
	}
	for (int32 i = 0; i < 10; ++i)
	{
		FVector RandPoint = UKismetMathLibrary::RandomPointInBoundingBox(Box->GetComponentLocation(), Box->GetUnscaledBoxExtent());
		if (GetWorld())
		{
			AExplosionEffect* TempEffect = GetWorld()->SpawnActorDeferred<AExplosionEffect>(ExplosionEffect, FTransform(FRotator::ZeroRotator, RandPoint), this);
			if (TempEffect)
			{
				TempEffect->DelayTime = FMath::FRand();
				UGameplayStatics::FinishSpawningActor(TempEffect, FTransform(FRotator::ZeroRotator, RandPoint));
			}
		}
	}
}

void ATDMap::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (ATDEnemy* Enemy = Cast<ATDEnemy>(OtherActor))
	{
		if (ATDController* MC = Cast<ATDController>(GetOwner()))
		{
			MC->AddMoney(-100);
			Enemy->Destroy();
		}
	}
}

