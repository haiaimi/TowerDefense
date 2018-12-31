// Fill out your copyright notice in the Description page of Project Settings.

#include "Missle.h"
#include <PaperSpriteComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/BoxComponent.h>
#include "TDTypes.h"
#include "ExplosionEffect.h"
#include <Engine/World.h>
#include "Common/HAIAIMIHelper.h"
#include "TDEnemy.h"


AMissle::AMissle()
{
	BulletFire = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletFire"));
	BulletFire->SetVisibility(false);
	ProjectileComponent->InitialSpeed = 0.f;   
	ProjectileComponent->MaxSpeed = 600.f;
	ProjectileComponent->Velocity = FVector(0.f, 0.f, 1.f);
	BulletFire->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(COLLISION_MISSLE);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block);
	
	BulletFire->TranslucencySortPriority = 4;
	Damage = 100.f;
}

void AMissle::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMissle::BeginPlay()
{
	Super::BeginPlay();

	BulletFire->AttachToComponent(ProjectileSprite, FAttachmentTransformRules::KeepRelativeTransform, TEXT("FireSocket"));
	BulletFire->SetRelativeRotation(FRotator(180.f, 0.f, 0.f));

}

void AMissle::Launch(FVector Veolcity)
{
	//Super::Launch(Veolcity);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	ProjectileComponent->Velocity = Veolcity;
	BulletFire->SetVisibility(true);
}

void AMissle::OnImpact(const FHitResult& result)
{
	FVector ImpactPoint = result.ImpactPoint;
	ImpactPoint.Y = 10.f;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	AExplosionEffect* Tmp = GetWorld()->SpawnActor<class AExplosionEffect>(ExplosionEffect, FTransform(FRotator::ZeroRotator, ImpactPoint + FVector(0.f, 50.f, 0.f)), SpawnParameters);

	TArray<FHitResult> Result;
	GetWorld()->SweepMultiByObjectType(Result, ImpactPoint, ImpactPoint + FVector(0.f, 100.f, 0.f), 
									  FQuat::Identity, 
									  FCollisionObjectQueryParams(COLLISION_ENEMY),      //检测得对象类型
									  FCollisionShape::MakeSphere(40.f));  //半径25得检测范围
	for (auto iter = Result.CreateIterator(); iter; ++iter)
	{
		if (ATDEnemy* Enemy = Cast<ATDEnemy>((*iter).GetActor()))
		{
			//对敌人附加伤害
			Enemy->TakeDamage(Damage, FDamageEvent(), NULL, NULL);
		}
	}

	Destroy();
}
