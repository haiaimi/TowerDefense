// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"
#include <SlateSound.h>

#include "MenuSoundWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FMenuSoundStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FMenuSoundStyle();
	virtual ~FMenuSoundStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FMenuSoundStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category=Sound)
	struct FSlateSound ButtonSound;

	UPROPERTY(EditAnywhere, Category=Sound)
	struct FSlateSound RankSound;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UMenuSoundWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FMenuSoundStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
