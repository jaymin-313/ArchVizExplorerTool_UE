// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadEditorWidget.h"
#include "ArchVizExplorerController.h"
#include <Components/TextBlock.h>
#include <Components/HorizontalBox.h>
void URoadEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (RoadWidth) {
		RoadWidth->OnValueChanged.AddDynamic(this, &URoadEditorWidget::OnRoadWidthChanged);
	}
	if (DestroyRoad) {
		DestroyRoad->OnClicked.AddDynamic(this, &URoadEditorWidget::OnDestroyRoad);
	}
	if (RoadLocationX) {
		RoadLocationX->OnValueChanged.AddDynamic(this, &URoadEditorWidget::OnRoadLocationXChanged);
	}
	if (RoadLocationY) {
		RoadLocationY->OnValueChanged.AddDynamic(this, &URoadEditorWidget::OnRoadLocationYChanged);
	}
}

void URoadEditorWidget::OnRoadWidthChanged(float InValue)
{
	if (PlayerController) {
		PlayerController->OnRoadWidthChanged(InValue);
	}
	
}

void URoadEditorWidget::OnRoadLocationXChanged(float InLocationX)
{
	if (PlayerController) {
		
		PlayerController->OnRoadLocationXChanged(InLocationX);
	}
}

void URoadEditorWidget::OnRoadLocationYChanged(float InLocationY)
{
	if (PlayerController) {
	
		PlayerController->OnRoadLocationYChanged(InLocationY);
	}
}

void URoadEditorWidget::OnDestroyRoad()
{
	if (PlayerController) {
		PlayerController->OnDestroyRoad();
	}
}

void URoadEditorWidget::SetPlayerController(AArchVizExplorerController* InController)
{
	PlayerController = InController;
}
