// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "Components/SpinBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include <Components/Image.h>
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include "RoadMaterialAsset.h"
#include <Components/CanvasPanel.h>

#include <Components/EditableText.h>
#include "ArchWizExplorerWidget.generated.h"

/**
 * 
 */

UCLASS()
class ARCHVIZEXPLORER_API UArchWizExplorerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ModeSelectionWidget;
	
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* SavedBuildingOptions;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* LoadProject;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SaveProject;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* ClearProject;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* FirstFloorButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* GroundFloorButton;

	UPROPERTY(meta = (BindWidget))
	UEditableText* NameOfBuilding;
	
	UPROPERTY(meta = (BindWidget))
	UEditableText* LoadBuildingName;

	UPROPERTY(meta = (BindWidget))
	class UButton* SaveNameButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentFloor;

	UPROPERTY(meta = (BindWidget))
	UBorder* MessageBorder;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* SaveBorder;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageTextBlock;

	UPROPERTY(meta = (BindWidget))
	UBorder* BuildingInstruction;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* GuideSwitcher;

	UPROPERTY(meta = (BindWidget))
	UBorder* RoadInstruction;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* InteriorInstruction;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* MaterialInstruction;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* ViewInstruction;

	UPROPERTY(meta = (BindWidget))
	UButton* ShowGuide;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GuideText;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanelStart;

	UPROPERTY(meta = (BindWidget))
	UButton* NewProject;
	UFUNCTION()
	void OnSelectionModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnLoadOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnSaveGame();
	UFUNCTION()
	void OnLoadGame();
	UFUNCTION()
	void OnClearGame();

	

	UFUNCTION()
	void OnNewProject();

	void SetPlayerController(class AArchVizExplorerController* InController);
	UFUNCTION()
	void OnFirstFloorSelected();

	UFUNCTION()
	void OnGroundFloorSelected();

	UFUNCTION()
	void OnSaveName();

	UFUNCTION()
	void OnLoadName();

	UFUNCTION()
	void PopulateComboBox(TArray<FString>& InMapNames);
	UFUNCTION()
	void OnShowGuide();
private:
	UPROPERTY()
	class AArchVizExplorerController* PlayerController;

};
