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
#include "../UI/Widgets/SHUDWidget.h"
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
	TowerTypes.SetNum(3);
	static ConstructorHelpers::FClassFinder<ATDMap> DefaultMapFinder(TEXT("/Game/Blueprint/maps/map1"));
	static ConstructorHelpers::FClassFinder<ATDTowerBase> TowerFinder1(TEXT("/Game/Blueprint/Weapon/Tower2Missle"));
	static ConstructorHelpers::FClassFinder<ATDTowerBase> TowerFinder2(TEXT("/Game/Blueprint/Weapon/TowerGun"));
	static ConstructorHelpers::FClassFinder<ATDTowerBase> TowerFinder3(TEXT("/Game/Blueprint/Weapon/Tower1Missle"));
	if (DefaultMapFinder.Succeeded())
		DefaultMap = DefaultMapFinder.Class;
	if (TowerFinder1.Succeeded())
		TowerTypes[0] = TowerFinder1.Class;
	if (TowerFinder2.Succeeded())
		TowerTypes[1] = TowerFinder2.Class;
	if (TowerFinder3.Succeeded())
		TowerTypes[2] = TowerFinder3.Class;

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
		CurMap->SetOwner(this);
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
	GetWorld()->GetTimerManager().SetTimer(InitPawnTimer, InitPawn, 0.1f, false);
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
	//CurMap->SpawnEnemy();
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
	else if (ScreenLocation.X > 1600.f && ScreenLocation.Y > 300.f && ScreenLocation.Y < 800.f)
		RotateAngle = -90.f;
	else if (ScreenLocation.X > 1700.f && ScreenLocation.Y > 800.f)
		RotateAngle = -45.f;
	else if (ScreenLocation.X > 300.f &&ScreenLocation.X < 1700.f && ScreenLocation.Y > 800.f)
		RotateAngle = 0.f;
	else if (ScreenLocation.X < 300.f && ScreenLocation.Y > 800.f)
		RotateAngle = 45.f;
	else if (ScreenLocation.X < 300.f && ScreenLocation.Y > 300.f && ScreenLocation.Y < 800.f)
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
		TSharedPtr<class SHUDWidget> ScoreWidget = CurHUD->GetHUDWidget();
		if (ScoreWidget.IsValid())
			ScoreWidget->AddScore(AddedMoney);
	}
}

int32 ATDController::GetSpecifiedTowerCost(int32 Index)
{
	return TowerTypes[Index].GetDefaultObject()->GetBuildCost();
}

bool ATDController::SpawnTower(const int32 TowerIndex, ATDTowerBase* BaseTower)
{
	int32 BuildCost = TowerTypes[TowerIndex].GetDefaultObject()->GetBuildCost();
	if (BuildCost > CurMoney)return false;
	if (!GetWorld())return false;
	ATDTowerBase* SpawnedTower = GetWorld()->SpawnActorDeferred<ATDTowerBase>(TowerTypes[TowerIndex], BaseTower->GetTransform(), BaseTower->GetOwner());
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
	if (ATDHUD* MHD = Cast<ATDHUD>(MyHUD))
	{
		MHD->GetHUDWidget()->SetWidgetPause(bPause);
	}

	if (bPause)
	{
		if (!PauseWidget.IsValid())
		{
			FSimpleDelegate BackMenuDelgate;
			BackMenuDelgate.BindLambda([&]() {
					if (GetWorld())
					{
						HAIAIMIHelper::SaveScore(CurScore);
						UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/GameLevels/Menu"));
					}
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
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/GameLevels/GameMap"), true);
	}
}
