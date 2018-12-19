// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDTypes.h"
#include "TDTowerBase.generated.h"

UCLASS()
class TOWERDEFENSE_API ATDTowerBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UPaperSpriteComponent* TowerSprite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UBoxComponent* TowerCollision;

	/**生命值*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	float Health;

	/**防御塔爆炸效果*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AExplosionEffect> TowerExpolsionEffect;

	ETowerType::Type TowerType;

public:	
	// Sets default values for this actor's properties
	ATDTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
};
