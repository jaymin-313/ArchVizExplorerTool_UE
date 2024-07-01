// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "ProcRoadActor.h"
#include "CustomBuildingActor.h"
#include "CustomInteriorActor.h"
#include "ArchWizExplorerWidget.h"
#include "RoadMaterialAsset.h"
#include <RoadEditorWidget.h>
#include "MaterialEditorWidget.h"
#include "BuildingEditorWidget.h"
#include "InteriorEditorWidget.h"
#include "ArchVizSaveGame.h"
#include "TimerManager.h"
#include "ArchVizExplorerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API AArchVizExplorerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void Tick(float DeltaTime);
	void ApplyMaterial(const FMaterialData& MaterialData);
	void SelectType(const FBuildingTypeData& BuildingData);
	bool CheckCollisionAtLocation(const FVector& Location, const FVector& ActorExtent, const TArray<FString>& IgnoredActorTypes, ACustomBuildingActor* ActorToIgnore = nullptr);
	bool IsInteriorOnBuildingActor(const FVector& Location, const FVector& ActorExtent, const TArray<FString>& IgnoredActorTypes, ACustomBuildingActor* ActorToIgnore);
	ACustomBuildingActor* GetCustomBuildingActor(const FVector& Location, const FVector& ActorExtent);
	bool CheckInteriorCollision(const FVector& Location, const FVector& ActorExtent, ACustomInteriorActor* ActorToIgnore);
	void PlaceInterior(const FInteriorData& InteriorData);
	void PlaceWallInterior(const FWallInteriorData& WallInteriorData);
	void PlaceCeilingInterior(const FCeilingInteriorData& CeilingInteriorData);
	void OnInteriorLocationXChanged(float InValue);
	void OnInteriorLocationYChanged(float InValue);
	void OnInteriorLocationZChanged(float InValue);
	void OnSaveGameClicked();
	void OnLoadGameClicked();
	void SaveGame(FString InName);
	void LoadGame(FString InName);
	void ClearSaveGameData();

	void RetrieveFilenamesFromDirectory(const FString& DirectoryPath, TArray<FString>& OutFilenames);

	FVector InteriorHitLocation;
	virtual void SetupInputComponent() override;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	UPROPERTY()
	ULocalPlayer* LocalPlayer;

	UPROPERTY()
	UInputMappingContext* RoadMapping;
	UPROPERTY()
	UInputMappingContext* BuildingMapping;
	UPROPERTY()
	UInputMappingContext* InteriorMapping;
	UPROPERTY()
	UInputMappingContext* MaterialMapping;

	UPROPERTY()
	class UInputAction* LeftClickAction;

	UPROPERTY()
	class UInputAction* M_LeftClickAction;

	UPROPERTY()
	class UInputAction* B_LeftClickAction;
	UPROPERTY()
	class UInputAction* B_RightClickAction;
	UPROPERTY()
	class UInputAction* RotateAction;

	UPROPERTY()
	class UInputAction* I_LeftClickAction;
	
	UPROPERTY()
	class UInputAction* SaveTemplateAction;
	
	UPROPERTY()
	class UInputAction* LoadTemplateAction;
	
	
	void OnRoadWidthChanged(float Width);
	void OnRoadLocationXChanged(float InLocation);
	void OnRoadLocationYChanged(float InLocation);
	void ShowGuide();
	void OnModeChanged(FString InSelectedItem);
	void ShowMessage(FString Message);
	void HideInstructionText();
	void ClearViewportWidgets();
	void OnDestroyRoad();
	void OnDestroyInterior();
	void OnRotateInterior();
	void OnDestroyCustomBuidling();
	void SetNumberOfSegments(int32 Number);
	void SetNumberOfColumns(int32 Number);
	void SetNumberOfRows(int32 Number);
	void SnapActor(float SnapValue);
	void DestroyUnplacedActor();
	void I_LeftMouseClick();
	FVector GetActorExtentFromMesh(UStaticMesh* Mesh) const;
	void FirstFloorSelected();
	void GroundFloorSelected();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI")
	TSubclassOf<UUserWidget> ArchVizWidgetClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI")
	TSubclassOf<UUserWidget> RoadWidgetClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI")
	TSubclassOf<UUserWidget> MaterialWidgetClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI")
	TSubclassOf<UUserWidget> BuildingWidgetClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI")
	TSubclassOf<UUserWidget> InteriorWidgetClass;



	UPROPERTY()
	UArchWizExplorerWidget* ArchVizWidgetInstance;
	UPROPERTY()
	URoadEditorWidget* RoadWidgetInstance;
	UPROPERTY()
	UMaterialEditorWidget* MaterialWidgetInstance;
	UPROPERTY()
	UBuildingEditorWidget* BuildingWidgetInstance;

	UPROPERTY()
	UInteriorEditorWidget* InteriorWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	URoadMaterialAsset* RoadMaterialAsset = LoadObject<URoadMaterialAsset>(this, TEXT("/Script/ArchVizExplorer.RoadMaterialAsset'/Game/RoadMaterialAsset.RoadMaterialAsset'"));

	void OnLeftMouseClick();
	void M_LeftMouseClick();
	void ApplyBuildingMaterial(const FBuildingMaterialData& BuildingMaterialData);
	void ChangeDoorMesh(const FDoorMeshTypeData& DoorData);
	void B_LeftMouseClick();
	void B_RightMouseClick();
	
	void SpawnCustomBuildingActor();
	void RotateCustomBuilding();
	int32 NumberOfSegments;
	 FVector FirstPoint;
	 FVector SecondPoint;
	 bool bIsFirstPointSelected;
	 bool bIsBuildingMappingContext = false;
	 bool bMove = true;
	 bool bIsPlaced = true;
	 bool bGuideVisible = false;

	 UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Road")
	 TSubclassOf<AProcRoadActor> ProcRoadClass;

	 UPROPERTY()
	 AProcRoadActor* ProceduralRoadActor;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	 TSubclassOf<ACustomBuildingActor>CustomBuildingClass;
	 UPROPERTY()
	 ACustomBuildingActor* CustomBuildingInstance;
	 
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	 TSubclassOf<ACustomInteriorActor> CustomInteriorClass;
	 UPROPERTY()
	 ACustomInteriorActor* CustomInteriorInstance;

	 FTimerHandle TimeHandle;

 private:

	 //First Floor
	 float i  = 0.0f;
	 int32 CurrentFloor{};
	 UPROPERTY()
	 TArray<FString> BuildingNames;
};
