// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDTypes.generated.h"

#define COLLISION_ENEMY        ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_TOWER        ECollisionChannel::ECC_GameTraceChannel2
#define COLLISION_TOWERBASE    ECollisionChannel::ECC_GameTraceChannel3
#define COLLISION_MISSLE       ECollisionChannel::ECC_GameTraceChannel4
#define COLLISION_ENEMYBULLET       ECollisionChannel::ECC_GameTraceChannel5

UENUM(BlueprintType)
namespace ETowerType
{
	enum Type
	{
		EBase,      
		ETower2Missle,
	};
}