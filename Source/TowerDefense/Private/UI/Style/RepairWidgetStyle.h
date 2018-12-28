// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"
#include <SlateBrush.h>

#include "RepairWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FRepairStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FRepairStyle();
	virtual ~FRepairStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FRepairStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush RepairIcon;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush RepairAction;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class URepairWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FRepairStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
