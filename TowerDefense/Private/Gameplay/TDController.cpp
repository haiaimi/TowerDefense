﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "TDController.h"
#include "TDMap.h"
#include <EngineUtils.h>
#include <ConstructorHelpers.h>
#include <Engine/Engine.h>
#include "Common/HAIAIMIHelper.h"
#include "TDTowerBase.h"
#include <Engine/LocalPlayer.h>
#include "UI/TDHUD.h"
#include "../UI/Widgets/SScoreWidget.h"
#include <Kismet/GameplayStatics.h>


ATDController::ATDController() :CurMap(nullptr)
{
	static ConstructorHelpers::FClassFinder<ATDMap> DefaultMapFinder(TEXT("/Game/Blueprint/maps/map1"));
	static ConstructorHelpers::FClassFinder<ATDTowerBase> TowerFinder(TEXT("/Game/Blueprint/Weapon/Tower2Missle"));
	if (DefaultMapFinder.Succeeded())
		DefaultMap = DefaultMapFinder.Class;
	if (TowerFinder.Succeeded())
		Tower1 = TowerFinder.Class;
}

void ATDController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;
	GetLocalPlayer()->ViewportClient->SetCaptureMouseOnClick(EMouseCaptureMode::CaptureDuringMouseDown);
	for (TActorIterator<ATDMap> Iter(GetWorld()); Iter; ++Iter)
	{
		CurMap = *Iter;
		break;
	}
	
	if (!CurMap && DefaultMap)  //如果不存在map就新生成一个
	{
		CurMap = GetWorld()->SpawnActor<ATDMap>(DefaultMap, FTransform(FRotator::ZeroRotator, FVector::ZeroVector));
	}
}

void ATDController::SetupInputComponent()
{
	Super::SetupInputComponent() ;

	InputComponent->BindAction("SpawnEnemy", IE_Pressed, this, &ATDController::SpawnEnemy);
	InputComponent->BindAction("ClickMap", IE_Pressed, this, &ATDController::DetectMap);
}

void ATDController::SpawnEnemy()
{
	CurMap->SpawnEnemy();
}

void ATDController::DetectMap()
{
	FVector WorldLocation, Dir;
	FVector2D MousePos;
	GetMousePosition(MousePos.X, MousePos.Y);
	HAIAIMIHelper::Debug_ScreenMessage(MousePos.ToString());
	
	FHitResult Result;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, Result))
	{
		ATDTowerBase* TowerBase = Cast<ATDTowerBase>(Result.GetActor());
		if (TowerBase && TowerBase->TowerType == ETowerType::EBase)
		{
			ATDTowerBase* Tmp = GetWorld()->SpawnActorDeferred<ATDTowerBase>(Tower1, TowerBase->GetTransform(), TowerBase->GetOwner());
			if (Tmp)
			{
				Tmp->InMapIndex = TowerBase->InMapIndex;
				UGameplayStatics::FinishSpawningActor(Tmp, TowerBase->GetTransform());
			}
			//if (ATDMap* OwnerMap = Cast<ATDMap>(TowerBase->GetOwner()))
			//	OwnerMap->UpdateTowerType(ETowerType::ETower2Missle, TowerBase->InMapIndex); //重新设置炮台类型
			TowerBase->Destroy();
			HAIAIMIHelper::Debug_ScreenMessage(TEXT("Hit Screen"));
		}
	}
}

void ATDController::AddScore(int32 AddedScore)
{
	if (ATDHUD* CurHUD = Cast<ATDHUD>(GetHUD()))
	{
		TSharedPtr<class SScoreWidget> ScoreWidget = CurHUD->GetScoreWidget();
		if (ScoreWidget.IsValid())
			ScoreWidget->AddScore(AddedScore);
	}
}
