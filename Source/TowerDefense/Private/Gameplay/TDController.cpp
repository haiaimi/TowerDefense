// Fill out your copyright notice in the Description page of Project Settings.

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
#include "../UI/Widgets/STowerSelectWidget.h"
#include "SPauseMenuWidget.h"
#include <Engine/World.h>
#include <GameFramework/GameModeBase.h>
#include "../UI/Widgets/SRepairWidget.h"
#include "TDPlayerCameraManager.h"
#include <TimerManager.h>

ATDController::ATDController() :
	CurMap(nullptr),
	CurMoney(1000),
	CurScore(0),
	TowerWidget(nullptr),
	PauseWidget(nullptr)
{
	static ConstructorHelpers::FClassFinder<ATDMap> DefaultMapFinder(TEXT("/Game/Blueprint/maps/map1"));
	static ConstructorHelpers::FClassFinder<ATDTowerBase> TowerFinder(TEXT("/Game/Blueprint/Weapon/Tower2Missle"));
	if (DefaultMapFinder.Succeeded())
		DefaultMap = DefaultMapFinder.Class;
	if (TowerFinder.Succeeded())
		Tower1 = TowerFinder.Class;

	PlayerCameraManagerClass = ATDPlayerCameraManager::StaticClass();
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
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		CurMap = GetWorld()->SpawnActor<ATDMap>(DefaultMap, FTransform(FRotator::ZeroRotator, FVector::ZeroVector), SpawnParameter);
	}

	FTimerDelegate InitPawn;
	InitPawn.BindLambda([&]() {
			if (GetPawn())
			{
				GetPawn()->DisableInput(this);
				GetPawn()->SetActorHiddenInGame(true);
			}
		});
	GetWorld()->GetTimerManager().SetTimer(InitPawnTimer, InitPawn, 1.f, false);
}

void ATDController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SpawnEnemy", IE_Pressed, this, &ATDController::SpawnEnemy);
	InputComponent->BindAction("ClickMap", IE_Pressed, this, &ATDController::DetectMap);
	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ATDController::PauseGame);
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
	
	FVector2D ScreenLocation = HAIAIMIHelper::ConvertToNormalCoord(MousePos);
	float RotateAngle = 0.f;
	if (ScreenLocation.X < 300.f && ScreenLocation.Y < 300.f)
		RotateAngle = 135.f;
	else if (ScreenLocation.X > 300.f &&ScreenLocation.X < 1600.f && ScreenLocation.Y < 300.f)
		RotateAngle = 180.f;
	else if (ScreenLocation.X > 1600.f && ScreenLocation.Y < 300.f)
		RotateAngle = -135.f;
	else if (ScreenLocation.X > 1600.f && ScreenLocation.Y > 300.f&&ScreenLocation.Y < 800.f)
		RotateAngle = -90.f;
	else if (ScreenLocation.X >1700.f && ScreenLocation.Y > 800.f)
		RotateAngle = -45.f;
	else if (ScreenLocation.X > 300.f &&ScreenLocation.X < 1700.f && ScreenLocation.Y > 800.f)
		RotateAngle = 0.f;
	else if (ScreenLocation.X < 300.f && ScreenLocation.Y > 800.f)
		RotateAngle = 45.f;
	else if (ScreenLocation.X < 300.f && ScreenLocation.Y > 300.f&&ScreenLocation.Y < 800.f)
		RotateAngle = 90.f;

	FHitResult Result;
	if (GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, Result))
	{
		ATDTowerBase* TowerBase = Cast<ATDTowerBase>(Result.GetActor());
		if (TowerBase && TowerBase->TowerType == ETowerType::EBase)
		{
			SAssignNew(TowerWidget, STowerSelectWidget)
				.StartPos(ScreenLocation)
				.CurController(this)
				.CurBaseTower(TowerBase)
				.RoatateAngle(RotateAngle);

			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SWeakWidget)
				.PossiblyNullContent(TowerWidget.ToSharedRef()),
				0
			);
		}
	}
	else
	{
		if (TowerWidget.IsValid())
			TowerWidget->ReserveButtons();
	}
}

void ATDController::AddMoney(int32 AddedMoney)
{
	CurMoney += AddedMoney;
	
	if (CurMoney < 0)
	{
		SetPause(true);
		PauseWidget->SetContinueButtonEnable(false);
		HAIAIMIHelper::SaveScore(CurScore);
		return;
	}
	if (ATDHUD* CurHUD = Cast<ATDHUD>(GetHUD()))
	{
		TSharedPtr<class SScoreWidget> ScoreWidget = CurHUD->GetScoreWidget();
		if (ScoreWidget.IsValid())
			ScoreWidget->AddScore(AddedMoney);
	}
}

int32 ATDController::GetSpecifiedTowerCost(int32 Index)
{
	return Tower1.GetDefaultObject()->GetBuildCost();
}

bool ATDController::SpawnTower(const int32 TowerIndex, ATDTowerBase* BaseTower)
{
	int32 BuildCost = Tower1.GetDefaultObject()->GetBuildCost();
	if (BuildCost > CurMoney)return false;
	if (!GetWorld())return false;
	ATDTowerBase* SpawnedTower = GetWorld()->SpawnActorDeferred<ATDTowerBase>(Tower1, BaseTower->GetTransform(), BaseTower->GetOwner());
	if (SpawnedTower)
	{
		SpawnedTower->InMapIndex = BaseTower->InMapIndex;
		UGameplayStatics::FinishSpawningActor(SpawnedTower, BaseTower->GetTransform());
		AddMoney(-BuildCost);
		BaseTower->Destroy();
	}

	return SpawnedTower != NULL;
}

void ATDController::ResetSelectWidget()
{
	if (TowerWidget.IsValid())
		TowerWidget.Reset();
}

bool ATDController::SetPause(bool bPause, FCanUnpause CanUnpauseDelegate /*= FCanUnpause()*/)
{
	if (bPause)
	{
		if (!PauseWidget.IsValid())
		{
			FSimpleDelegate BackMenuDelgate;
			BackMenuDelgate.BindLambda([&]() {
				if (GetWorld())
				{
					GetWorld()->ServerTravel(TEXT("/Game/Levels/Menu"), false, true);
				}
				HAIAIMIHelper::SaveScore(CurScore);
				});

			SAssignNew(PauseWidget, SPauseMenuWidget)
				.OwnerController(this)
				.BackDelegate(BackMenuDelgate);

			GEngine->GameViewport->AddViewportWidgetContent(
				SNew(SWeakWidget)
				.PossiblyNullContent(PauseWidget.ToSharedRef()),
				0
			);
		}
		else
		{
			PauseWidget->SetVisibility(EVisibility::Visible);
			PauseWidget->SetEnabled(true);
		}
	}
	else
	{
		PauseWidget->SetVisibility(EVisibility::Hidden);
			PauseWidget->SetEnabled(false);
	}

	for (TActorIterator<ATDTowerBase> Iter(GetWorld()); Iter; ++Iter)
	{
		if((*Iter)->RepairWidget.IsValid())
		{
			(*Iter)->RepairWidget->SetVisibility(bPause ? EVisibility::Hidden : EVisibility::Visible);
		}
	}
	
	return Super::SetPause(bPause);
}

void ATDController::RestartGame()
{
	if (GetWorld())
	{
		//这里要提前删除，否则游戏会崩溃
		for (TActorIterator<ATDTowerBase> Iter(GetWorld()); Iter; ++Iter)
		{
			(*Iter)->Destroy();
		}
		GetWorld()->ServerTravel(TEXT("/Game/Levels/GameMap"), false, true);
	}
}
