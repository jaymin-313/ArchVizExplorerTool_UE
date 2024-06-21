// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/SpinBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include <Components/Image.h>
#include "RoadMaterialAsset.h"
#include "RoadEditorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API URoadEditorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	class USpinBox* RoadWidth;

	UPROPERTY(meta = (BindWidget))
	class USpinBox* RoadLocationX;

	UPROPERTY(meta = (BindWidget))
	class USpinBox* RoadLocationY;


	UPROPERTY(meta = (BindWidget))
	class UButton* DestroyRoad;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	URoadMaterialAsset* RoadMaterialDataAsset = LoadObject<URoadMaterialAsset>(this, TEXT("/Script/ArchVizExplorer.RoadMaterialAsset'/Game/RoadMaterialAsset.RoadMaterialAsset'"));

	//For Road 
	UFUNCTION()
	void OnRoadWidthChanged(float InValue);
	UFUNCTION()
	void OnRoadLocationXChanged(float InLocationX);
	UFUNCTION()
	void OnRoadLocationYChanged(float InLocationY);



	UFUNCTION()
	void OnDestroyRoad();

	void SetPlayerController(class AArchVizExplorerController* InController);



private:
	UPROPERTY()
	class AArchVizExplorerController* PlayerController;

	TMap<UButton*, UMaterialInterface*> MaterialButtonMap;
};
