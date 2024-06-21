// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxWidget.h"
#include "Components/Button.h"
#include <Components/SpinBox.h>
#include "BuildingEditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UBuildingEditorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnNumSegmentsChanged(float Number);
	UFUNCTION()
	void OnDestroyClicked();
	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* BuildingScrollBox;

	UPROPERTY(meta = (BindWidget))
	USpinBox* NumOfSegments;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyComponent;
};
