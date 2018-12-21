// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"
#include <SlateBrush.h>

#include "TowerSelectWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FTowerSelectStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FTowerSelectStyle();
	virtual ~FTowerSelectStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FTowerSelectStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush  Border;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush TwoMissle;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush  OneMissle;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush  Cannon;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UTowerSelectWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FTowerSelectStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
