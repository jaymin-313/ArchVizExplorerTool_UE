// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoadMaterialAsset.h"
#include "Components/Button.h"
#include "ScrollBoxWidget.h"
#include "MaterialEditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UMaterialEditorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UScrollBoxWidget* MaterialScrollBox;

    UPROPERTY(meta = (BindWidget))
    UScrollBoxWidget* BuildingMaterialBox;
};
