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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHealth;

	/**生命值*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;

	/**防御塔爆炸效果*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AExplosionEffect> TowerExpolsionEffect;

	/**炮台损坏的冒烟动画*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AExplosionEffect> InjureSmoke;

	ETowerType::Type TowerType;

	/**在地图中是第几个炮台*/
	int32 InMapIndex;

	TSharedPtr<class SRepairWidget> RepairWidget;

	FTimerHandle InjuredTimer;

	/**治疗定时器*/
	FTimerHandle HealTimer;

private:
	/**建造所需消耗的金币*/
	int32 BuildCost;

	class ATDHUD* HUDRef;

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

	int32 GetBuildCost() { return BuildCost; }

	virtual void OnInjured();

	virtual void HealSelf();
};
