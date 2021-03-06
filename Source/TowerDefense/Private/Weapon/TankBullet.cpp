// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBullet.h"
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>
#include "TDTypes.h"
#include "TDTowerBase.h"
#include <Engine/World.h>
#include "ExplosionEffect.h"


ATankBullet::ATankBullet()
{
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(COLLISION_ENEMYBULLET);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_TOWER, ECollisionResponse::ECR_Block);
}

void ATankBullet::OnImpact(const FHitResult& result)
{
	FVector ImpactPoint = result.ImpactPoint;
	ImpactPoint.Y = 10.f;
	AExplosionEffect* Tmp = GetWorld()->SpawnActor<class AExplosionEffect>(ExplosionEffect, FTransform(FRotator::ZeroRotator, ImpactPoint + FVector(0.f, 50.f, 0.f)));


	if (ATDTowerBase* Tower = Cast<ATDTowerBase>(result.GetActor()))
	{
		Tower->TakeDamage(Damage, FDamageEvent(), NULL, NULL);
	}
	
	Destroy();
}
