// Fill out your copyright notice in the Description page of Project Settings.

#include "TDBomber.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "HAIAIMIHelper.h"
#include "Components/SceneComponent.h"


// Sets default values
ATDBomber::ATDBomber():
	MoveDir(-25.f,0.f,0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Planes.Init(nullptr, 3);
	PlaneShadows.Init(nullptr, 3);
}

// Called when the game starts or when spawned
void ATDBomber::BeginPlay()
{
	Super::BeginPlay();

	TArray<UPaperSprite*> PlaneSprites; PlaneSprites.Init(nullptr, 3);
	TArray<UPaperSprite*> PlaneShadowSprites;PlaneShadowSprites.Init(nullptr, 3);
	
	PlaneSprites[0] = LoadObject<UPaperSprite>(this, TEXT("/Game/Weapon/Sprite/towerDefense_tile271_Sprite"));
	PlaneShadowSprites[0] = LoadObject<UPaperSprite>(this, TEXT("/Game/Weapon/Sprite/towerDefense_tile294_Sprite"));
	PlaneSprites[1] = LoadObject<UPaperSprite>(this, TEXT("/Game/Weapon/Sprite/towerDefense_tile270_Sprite"));
	PlaneShadowSprites[1] = LoadObject<UPaperSprite>(this, TEXT("/Game/Weapon/Sprite/towerDefense_tile293_Sprite"));
	PlaneSprites[2] = PlaneSprites[1];
	PlaneShadowSprites[2] = PlaneShadowSprites[1];

	for (int32 i = 0; i < 3; ++i)
	{
		Planes[i] = NewObject<UPaperSpriteComponent>(this);
		if (Planes[i])
		{
			Planes[i]->SetSprite(PlaneSprites[i]);
			Planes[i]->TranslucencySortPriority = 8.f;
			Planes[i]->RegisterComponentWithWorld(GetWorld());
			Planes[i]->RegisterComponent();
			if (i != 0)Planes[i]->AttachToComponent(Planes[0], FAttachmentTransformRules::KeepRelativeTransform);
		}
		PlaneShadows[i] = NewObject<UPaperSpriteComponent>(this);
		if (PlaneShadows[i])
		{
			PlaneShadows[i]->SetSprite(PlaneShadowSprites[i]);
			PlaneShadows[i]->SetupAttachment(Planes[i]);
			PlaneShadows[i]->SetRelativeLocation(FVector(-150.f, 0.f, -100.f));
			PlaneShadows[i]->TranslucencySortPriority = 8.f;
			PlaneShadows[i]->RegisterComponentWithWorld(GetWorld());
			PlaneShadows[i]->RegisterComponent();
		}
	}
	
	RootComponent = Planes[0];
	Planes[1]->SetRelativeLocation(FVector(-250.f, 0.f, 300.f));
	Planes[2]->SetRelativeLocation(FVector(-250.f, 0.f, -300.f));
	
	SetActorLocation(FVector(0.f, 300.f, 0.f));
	SetActorRotation(MoveDir);
	SetActorScale3D(FVector(2.f, 2.f, 2.f));
}

// Called every frame
void ATDBomber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + 700.f*DeltaTime*MoveDir.Vector());
}

