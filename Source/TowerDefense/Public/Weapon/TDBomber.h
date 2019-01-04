// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDBomber.generated.h"

UCLASS()
class TOWERDEFENSE_API ATDBomber : public AActor
{
	GENERATED_BODY()
	
public:
	/**·É»úSprites*/
	TArray<class UPaperSpriteComponent*> Planes;

	/**·É»úÒõÓ°Sprites*/
	TArray<class UPaperSpriteComponent*> PlaneShadows;
	
	FRotator MoveDir;
public:	
	// Sets default values for this actor's properties
	ATDBomber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
