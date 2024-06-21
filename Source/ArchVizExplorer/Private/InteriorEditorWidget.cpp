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
	if (RotateInterior) {
		RotateInterior->OnClicked.AddDynamic(this, &UInteriorEditorWidget::OnRotateInterior);

	}
}

void UInteriorEditorWidget::OnDestroyInterior()
{

	AArchVizExplorerController* PlayerController = Cast<AArchVizExplorerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) {
		PlayerController->OnDestroyInterior();
		
	}
	
}
void UInteriorEditorWidget::OnRotateInterior() {
	AArchVizExplorerController* PlayerController = Cast<AArchVizExplorerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) {
		PlayerController->OnRotateInterior();

	}
}
void UInteriorEditorWidget::OnInteriorLocationXChanged(float InValue)
{
	AArchVizExplorerController* PlayerController = Cast<AArchVizExplorerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) {
		PlayerController->OnInteriorLocationXChanged(InValue);

	}
}

void UInteriorEditorWidget::OnInteriorLocationYChanged(float InValue)
{
	AArchVizExplorerController* PlayerController = Cast<AArchVizExplorerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController) {
		PlayerController->OnInteriorLocationYChanged(InValue);

	}
}
