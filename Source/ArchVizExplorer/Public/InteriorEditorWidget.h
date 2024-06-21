// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollBoxWidget.h"
#include <Components/Button.h>
#include <Components/SpinBox.h>
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

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* InteriorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* WallInteriorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UScrollBoxWidget* CeilingInteriorScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyInterior;

	UPROPERTY(meta = (BindWidget))
	UButton* RotateInterior;

	UPROPERTY(meta = (BindWidget))
	USpinBox * InteriorLocationX;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox * InteriorLocationY;
};
