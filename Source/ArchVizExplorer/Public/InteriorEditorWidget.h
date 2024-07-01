// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxWidget.h"
#include <Components/Button.h>
#include <Components/SpinBox.h>
#include <Components/GridPanel.h>  
#include <Components/Border.h>  
#include "InteriorEditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorEditorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnDestroyInterior();
	UFUNCTION()
	void OnRotateInterior();
	UFUNCTION()
	void OnInteriorLocationXChanged(float InValue);
	UFUNCTION()
	void OnInteriorLocationYChanged(float InValue);
	UFUNCTION()
	void OnInteriorLocationZChanged(float InValue);

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* InteriorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* WallInteriorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* CeilingInteriorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyInterior;

	UPROPERTY(meta = (BindWidget))
	UBorder* LocationBorder;
	
	UPROPERTY(meta = (BindWidget))
	UGridPanel* LocationGridPanel;

	UPROPERTY(meta = (BindWidget))
	UButton* RotateInterior;

	UPROPERTY(meta = (BindWidget))
	USpinBox * InteriorLocationX;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox * InteriorLocationY;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox * InteriorLocationZ;

	void SetPlayerController(class AArchVizExplorerController* InController);

private:
	UPROPERTY()
	class AArchVizExplorerController* PlayerController;
};
