// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingEditorWidget.h"
#include "ArchVizExplorerController.h"
#include <Kismet/GameplayStatics.h>

void UBuildingEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (NumOfSegments) {
		NumOfSegments->OnValueChanged.AddDynamic(this, &UBuildingEditorWidget::OnNumSegmentsChanged);
	}
	if (DestroyComponent) {
		DestroyComponent->OnClicked.AddDynamic(this, &UBuildingEditorWidget::OnDestroyClicked);
	}
	if (NumOfColumns) {
		NumOfColumns->OnValueChanged.AddDynamic(this, &UBuildingEditorWidget::OnNumOfColumnsChanged);
	}
	if (NumOfRows) {
		NumOfRows->OnValueChanged.AddDynamic(this, &UBuildingEditorWidget::OnNumOfRowsChanged);
	}
}

void UBuildingEditorWidget::OnNumSegmentsChanged(float Number)
{
	if (PlayerController)
	{
		PlayerController->SetNumberOfSegments(static_cast<int32>(Number));
	}
}

void UBuildingEditorWidget::OnDestroyClicked() {
	if (PlayerController)
	{
		PlayerController->OnDestroyCustomBuidling();
	}
}

void UBuildingEditorWidget::OnNumOfColumnsChanged(float InValue) {
	if (PlayerController)
	{
		PlayerController->SetNumberOfColumns(static_cast<int32>(InValue));
	}
}

void UBuildingEditorWidget::OnNumOfRowsChanged(float InValue) {
	if (PlayerController)
	{
		PlayerController->SetNumberOfRows(static_cast<int32>(InValue));
	}
}

void UBuildingEditorWidget::SetPlayerController(AArchVizExplorerController* InController)
{
	PlayerController = InController;
}
