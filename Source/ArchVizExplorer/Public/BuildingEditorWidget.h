// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxWidget.h"
#include "Components/Button.h"
#include <Components/SpinBox.h>
#include <Components/Border.h>
#include <Components/HorizontalBox.h>
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
	UFUNCTION()
	void OnNumOfColumnsChanged(float InValue);
	UFUNCTION()
	void OnNumOfRowsChanged(float InValue);


	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* BuildingScrollBox;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SegmentBox;
	UPROPERTY(meta = (BindWidget))
	UBorder* SegmentsBorder;

	UPROPERTY(meta = (BindWidget))
	USpinBox* NumOfSegments;

	UPROPERTY(meta = (BindWidget))
	UBorder* TwoDimensionBorder;

	UPROPERTY(meta = (BindWidget))
	USpinBox* NumOfColumns;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* NumOfRows;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyComponent;

	void SetPlayerController(class AArchVizExplorerController* InController);
private:
	UPROPERTY()
	class AArchVizExplorerController* PlayerController;
};
