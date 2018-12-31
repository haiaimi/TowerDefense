// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBullet.h"
#include <PaperSpriteComponent.h>
#include <Components/BoxComponent.h>
#include "TDTypes.h"



ATankBullet::ATankBullet()
{
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollision->SetCollisionObjectType(COLLISION_ENEMYBULLET);
	ProjectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ProjectileCollision->SetCollisionResponseToChannel(COLLISION_TOWER, ECollisionResponse::ECR_Block);
}
