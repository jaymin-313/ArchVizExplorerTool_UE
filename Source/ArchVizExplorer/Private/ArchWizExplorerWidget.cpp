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
	if (SavedBuildingOptions) {
		SavedBuildingOptions->OnSelectionChanged.AddDynamic(this, &UArchWizExplorerWidget::OnLoadOptionChanged);
	}
	if (SaveProject) {
		SaveProject->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnSaveGame);
	}
	if (LoadProject) {
		LoadProject->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnLoadGame);
	}
	if (ClearProject) {
		ClearProject->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnClearGame);
	}
	if (FirstFloorButton) {
		FirstFloorButton->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnFirstFloorSelected);
	}
	if (GroundFloorButton) {
		GroundFloorButton->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnGroundFloorSelected);
	}
	if (SaveNameButton) {
		SaveNameButton->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnSaveName);
	}
	if (LoadButton) {
		LoadButton->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnLoadName);
	}
	if (ShowGuide) {
		ShowGuide->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnShowGuide);
	}
	if (NewProject) {
		NewProject->OnClicked.AddDynamic(this, &UArchWizExplorerWidget::OnNewProject);
	}
	
}

void UArchWizExplorerWidget::OnSelectionModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (PlayerController) {
		PlayerController->OnModeChanged(SelectedItem);
	}
}
void UArchWizExplorerWidget::OnLoadOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {
	if (PlayerController) {
		PlayerController->LoadGame(SelectedItem);
		CanvasPanelStart->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UArchWizExplorerWidget::OnSaveGame() {
	if (PlayerController) {
		PlayerController->OnSaveGameClicked();
	}
}

void UArchWizExplorerWidget::OnLoadGame() {
	if (PlayerController) {
		PlayerController->OnLoadGameClicked();
		
	}
}

void UArchWizExplorerWidget::OnClearGame() {
	if (PlayerController) {
		PlayerController->ClearSaveGameData();
	}
}

void UArchWizExplorerWidget::OnNewProject()
{
	CanvasPanelStart->SetVisibility(ESlateVisibility::Collapsed);
}
void UArchWizExplorerWidget::SetPlayerController(AArchVizExplorerController* InController)
{
	PlayerController = InController;
}

void UArchWizExplorerWidget::OnFirstFloorSelected() {
	PlayerController->FirstFloorSelected();

}
void UArchWizExplorerWidget::OnGroundFloorSelected() {
	PlayerController->GroundFloorSelected();
}

void UArchWizExplorerWidget::OnSaveName() {
	FString Name = NameOfBuilding->GetText().ToString();
	PlayerController->SaveGame(Name);
}
void UArchWizExplorerWidget::OnLoadName() {
	FString Name = LoadBuildingName->GetText().ToString();
	PlayerController->LoadGame(Name);

}

void UArchWizExplorerWidget::PopulateComboBox(TArray<FString>& InMapNames)
{
	if (SavedBuildingOptions) {
		SavedBuildingOptions->ClearOptions();
		for (const FString& MapName : InMapNames)
		{
			SavedBuildingOptions->AddOption(MapName);
			
	        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, MapName);
			
		}
		//SavedBuildingOptions->RefreshOptions(); //
		//ModeSelectionWidget->SetSelectedIndex(2);
	}
}

void UArchWizExplorerWidget::OnShowGuide() {
	if (PlayerController) {
		PlayerController->ShowGuide();
	}
}
