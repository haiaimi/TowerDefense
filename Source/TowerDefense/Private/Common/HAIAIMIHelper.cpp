﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Common/HAIAIMIHelper.h"
#include "TowerDefense.h"
#include <Engine/Engine.h>
#include <Engine/GameViewportClient.h>
#include <Kismet/GameplayStatics.h>
#include "ScoreSaveGame.h"
#include "Paths.h"
#include "FileHelper.h"
#include "JsonSerializer.h"
#include "JsonValue.h"

FString HAIAIMIHelper::RelativePath = FString(TEXT("LevelConfig"));
TArray<TSharedPtr<FJsonValue>> HAIAIMIHelper::JsonParser = {};

void HAIAIMIHelper::Debug_ScreenMessage(FString&& InString, float ShowTime, FColor FontColor)
{
	GEngine->AddOnScreenDebugMessage(-1, ShowTime, FontColor, InString);
}

void HAIAIMIHelper::Debug_LogMessage(FString&& InString)
{
	UE_LOG(LogHM, Log, TEXT("%s"), *InString);
}

float HAIAIMIHelper::AdaptSubAngle(const float InAngle1, const float InAngle2)
{
	const float TempSub = InAngle1 - InAngle2;

	if (TempSub > 180.f)
		return 360.f - TempSub;
	else if (TempSub < -180.f)
		return 360.f + TempSub;
	else return TempSub;
}

float HAIAIMIHelper::AdaptAngle(const float InAngle)
{
	if (InAngle > 180.f)
		return 360.f - InAngle;
	else if (InAngle < -180.f)
		return 360.f + InAngle;
	else return InAngle;
}

FVector2D HAIAIMIHelper::ConvertToNormalCoord(FVector2D Pos)
{
	FVector2D Res = Pos;
	if (GEngine&&GEngine->GameViewport)
	{
		FVector2D Size;
		GEngine->GameViewport->GetViewportSize(Size);
		Res.X = Pos.X * 1920.f / Size.X;
		Res.Y = Pos.Y * 1080.f / Size.Y;
	}
	
	return Res;
}

void HAIAIMIHelper::SaveScore(uint32 newScore)
{
	UScoreSaveGame* CurSaveGame = NewObject<UScoreSaveGame>();
	CurSaveGame->AllScores = HAIAIMIHelper::LoadScores();
	TArray<uint32>& Temp = CurSaveGame->AllScores;

	for (int32 i = 0; i < 10; ++i)
	{
		if (newScore > Temp[i])
		{
			Temp.Insert(newScore, i);
			Temp.SetNum(10);
			break;
		}
	}
	UGameplayStatics::SaveGameToSlot(CurSaveGame, TEXT("AllScores"), 0);
}

TArray<uint32>  HAIAIMIHelper::LoadScores()
{
	TArray<uint32> Res;
	Res.Init(0, 10);
	if (USaveGame* Scores = UGameplayStatics::LoadGameFromSlot(TEXT("AllScores"), 0))
	{
		UScoreSaveGame* MyScores = Cast<UScoreSaveGame>(Scores);
		if (MyScores->AllScores.Num() == 0)
			return Res;
		else
			return MyScores->AllScores;
	}

	return Res;
}

bool HAIAIMIHelper::LoadStringFromFile(const FString& FileName, FString& OutInfo)
{
	if (FileName.IsEmpty())return false;

	FString AbsolutePath = FPaths::ProjectContentDir() + RelativePath + "/" + FileName;
	if (FPaths::FileExists(AbsolutePath))
	{
		if (FFileHelper::LoadFileToString(OutInfo, *AbsolutePath))
			return true;      //加载成功
		else
			Debug_LogMessage(TEXT("未找到文件"));
	}

	return false;
}

void HAIAIMIHelper::PrepareJson(const FString& FileName)
{
	FString JsonContent;
	//CurParseFile = FileName;
	if (LoadStringFromFile(FileName, JsonContent))
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonContent);       //创建Json读取实例类

		if (FJsonSerializer::Deserialize(JsonReader, JsonParser))    //反序列化
			Debug_LogMessage(TEXT("读取成功"));
		else
			Debug_LogMessage(TEXT("读取失败"));
	}
}

void HAIAIMIHelper::GetNumberFromJson(const FString& FileName, const FString& Key, const int32 Index, float& OutNumber)
{
	if (JsonParser.Num() > 0)
	{
		OutNumber = JsonParser[Index]->AsObject()->GetNumberField(Key);
		//JsonParser[Index]->AsObject()->GetNumberField()
	}
	else
		Debug_LogMessage(TEXT("请重新读取文件"));
}
