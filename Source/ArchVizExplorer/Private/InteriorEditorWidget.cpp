// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorEditorWidget.h"
#include "ArchVizExplorerController.h"
#include <Kismet/GameplayStatics.h>

void UInteriorEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (DestroyInterior) {
		DestroyInterior->OnClicked.AddDynamic(this, &UInteriorEditorWidget::OnDestroyInterior);
	}
	if (InteriorLocationX) {
		InteriorLocationX->OnValueChanged.AddDynamic(this, &UInteriorEditorWidget::OnInteriorLocationXChanged);
	}
	if (InteriorLocationY) {
		InteriorLocationY->OnValueChanged.AddDynamic(this, &UInteriorEditorWidget::OnInteriorLocationYChanged);
	}
	if (InteriorLocationY) {
		InteriorLocationZ->OnValueChanged.AddDynamic(this, &UInteriorEditorWidget::OnInteriorLocationZChanged);
	}
	if (RotateInterior) {
		RotateInterior->OnClicked.AddDynamic(this, &UInteriorEditorWidget::OnRotateInterior);
	}
}

void UInteriorEditorWidget::OnDestroyInterior()
{

	if (PlayerController) {
		PlayerController->OnDestroyInterior();
	}
	
}
void UInteriorEditorWidget::OnRotateInterior() {
	if (PlayerController) {
		PlayerController->OnRotateInterior();
	}
}
void UInteriorEditorWidget::OnInteriorLocationXChanged(float InValue)
{
	if (PlayerController) {
		PlayerController->OnInteriorLocationXChanged(InValue);
	}
}

void UInteriorEditorWidget::OnInteriorLocationYChanged(float InValue)
{
	if (PlayerController) {
		PlayerController->OnInteriorLocationYChanged(InValue);
	}
}

void UInteriorEditorWidget::OnInteriorLocationZChanged(float InValue)
{
	if (PlayerController) {
		PlayerController->OnInteriorLocationZChanged(InValue);
	}
}

void UInteriorEditorWidget::SetPlayerController(AArchVizExplorerController* InController)
{
	PlayerController = InController;
}
