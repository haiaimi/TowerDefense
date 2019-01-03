// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerBullet.h"
#include <Components/BoxComponent.h>
#include "TDTypes.h"
#include "TDEnemy.h"
#include "ExplosionEffect.h"
#include <Engine/World.h>



ATowerBullet::ATowerBullet()
{
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(COLLISION_MISSLE);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block);
}

void ATowerBullet::OnImpact(const FHitResult& result)
{
	FVector ImpactPoint = result.ImpactPoint;
	ImpactPoint.Y = 10.f;
	AExplosionEffect* Tmp = GetWorld()->SpawnActor<class AExplosionEffect>(ExplosionEffect, FTransform(FRotator::ZeroRotator, ImpactPoint + FVector(0.f, 50.f, 0.f)));

	if (ATDEnemy* Enemy = Cast<ATDEnemy>(result.GetActor()))
	{
		Enemy->TakeDamage(Damage, FDamageEvent(), NULL, NULL);
	}
	
	Destroy();
}
