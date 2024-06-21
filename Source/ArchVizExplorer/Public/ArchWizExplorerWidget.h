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
#include "RoadMaterialAsset.h"

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
	


	UFUNCTION()
	void OnSelectionModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	

	void SetPlayerController(class AArchVizExplorerController* InController);

private:
	UPROPERTY()
	class AArchVizExplorerController* PlayerController;

};
