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

ATDController::ATDController() :
	CurMap(nullptr),
	CurScore(1000),
	TowerWidget(nullptr),
	PauseWidget(nullptr)
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
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.Owner = this;
		CurMap = GetWorld()->SpawnActor<ATDMap>(DefaultMap, FTransform(FRotator::ZeroRotator, FVector::ZeroVector), SpawnParameter);
	}

	HAIAIMIHelper::SaveScore();
}

void ATDController::SetupInputComponent()
{
	Super::SetupInputComponent() ;

	InputComponent->BindAction("SpawnEnemy", IE_Pressed, this, &ATDController::SpawnEnemy);
	InputComponent->BindAction("ClickMap", IE_Pressed, this, &ATDController::DetectMap);
	InputComponent->BindAction("PauseGame", IE_Pressed, this, &ATDController::PauseGame);
}

void ATDController::SpawnEnemy()
{
	CurMap->SpawnEnemy();

	//GetWorld()->ServerTravel("/Game/Levels/Menu");
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
			SAssignNew(TowerWidget, STowerSelectWidget)
				.StartPos(HAIAIMIHelper::ConvertToNormalCoord(MousePos))
				.CurController(this)
				.CurBaseTower(TowerBase);

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

void ATDController::AddScore(int32 AddedScore)
{
	CurScore += AddedScore;
	if (CurScore < 0)
	{
		SetPause(true);
		return;
	}
	if (ATDHUD* CurHUD = Cast<ATDHUD>(GetHUD()))
	{
		TSharedPtr<class SScoreWidget> ScoreWidget = CurHUD->GetScoreWidget();
		if (ScoreWidget.IsValid())
			ScoreWidget->AddScore(AddedScore);
	}
}

int32 ATDController::GetSpecifiedTowerCost(int32 Index)
{
	return Tower1.GetDefaultObject()->GetBuildCost();
}

bool ATDController::SpawnTower(const int32 TowerIndex, ATDTowerBase* BaseTower)
{
	HAIAIMIHelper::Debug_ScreenMessage(FString::FormatAsNumber(CurScore));
	int32 BuildCost = Tower1.GetDefaultObject()->GetBuildCost();
	if (BuildCost > CurScore)return false;
	if (!GetWorld())return false;
	ATDTowerBase* SpawnedTower = GetWorld()->SpawnActorDeferred<ATDTowerBase>(Tower1, BaseTower->GetTransform(), BaseTower->GetOwner());
	if (SpawnedTower)
	{
		SpawnedTower->InMapIndex = BaseTower->InMapIndex;
		UGameplayStatics::FinishSpawningActor(SpawnedTower, BaseTower->GetTransform());
		AddScore(-BuildCost);
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
					GetWorld()->ServerTravel(TEXT("/Game/Levels/Menu"));
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
	
	return Super::SetPause(bPause);
}

void ATDController::RestartGame()
{
	if (GetWorld())
		GetWorld()->ServerTravel(TEXT("/Game/Levels/GameMap"));
}
