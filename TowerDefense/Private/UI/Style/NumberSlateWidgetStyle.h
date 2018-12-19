// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"
#include <SlateBrush.h>
#include "NumberSlateWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FNumberSlateStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FNumberSlateStyle();
	virtual ~FNumberSlateStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FNumberSlateStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category = Appearance)
	TArray<struct FSlateBrush> Numbers;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush  MoneyCoin;

};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UNumberSlateWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FNumberSlateStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
