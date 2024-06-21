// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchWizExplorerWidget.h"
#include "ArchVizExplorerController.h"
#include <Components/TextBlock.h>
#include <Components/HorizontalBox.h>


void UArchWizExplorerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ModeSelectionWidget) {
		ModeSelectionWidget->OnSelectionChanged.AddDynamic(this, &UArchWizExplorerWidget::OnSelectionModeChanged);
	}
	
}

void UArchWizExplorerWidget::OnSelectionModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PlayerController) {
		PlayerController->OnModeChanged(SelectedItem);
	}
}



void UArchWizExplorerWidget::SetPlayerController(AArchVizExplorerController* InController)
{
	PlayerController = InController;
}





