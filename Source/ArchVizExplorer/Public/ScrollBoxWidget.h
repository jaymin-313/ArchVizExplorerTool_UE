// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SSelectionScrollBoxWidget.h"
#include "ScrollBoxWidget.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnMaterialSelected, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FOnBuildingSelected, const FBuildingTypeData&)
DECLARE_DELEGATE_OneParam(FOnInteriorSelected, const FInteriorData&)
DECLARE_DELEGATE_OneParam(FOnWallInteriorSelected, const FWallInteriorData&)
DECLARE_DELEGATE_OneParam(FOnCeilingInteriorSelected, const FCeilingInteriorData&)
DECLARE_DELEGATE_OneParam(FOnBuildingMaterialSelected, const FBuildingMaterialData&)
DECLARE_DELEGATE_OneParam(FOnDoorSelected, const FDoorMeshTypeData&)

UCLASS()
class ARCHVIZEXPLORER_API UScrollBoxWidget : public UWidget
{
	GENERATED_BODY()
public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void HandleMaterialSelection(const FMaterialData& MaterialData);
	void HandleBuildingTypeSelection(const FBuildingTypeData& BuildingData);
	void HandleDoorTypeSelection(const FDoorMeshTypeData& DoorData);
	void HandleInteriorTypeSelection(const FInteriorData& InteriorData);
	void HandleWallInteriorTypeSelection(const FWallInteriorData& WallInteriorData);
	void HandleCeilingInteriorTypeSelection(const FCeilingInteriorData& CeilingInteriorData);
	void HandleBuildingMaterialSelection(const FBuildingMaterialData& BuildingMaterialData); 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Asset Manager")
	URoadMaterialAsset* MeshAssetManger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Type")
	TEnumAsByte<EAssetType> AssetType;

	void ReleaseSlateResources(bool bReleaseChildren);
	const FText GetPaletteCategory();
	FOnMaterialSelected MaterialSelected;
	FOnBuildingSelected BuildingSelected;
	FOnInteriorSelected InteriorSelected;
	FOnWallInteriorSelected WallInteriorSelected;
	FOnCeilingInteriorSelected CeilingInteriorSelected;
	FOnBuildingMaterialSelected BuildingMaterialSelected;
	FOnDoorSelected DoorSelected;
private:
	TSharedPtr<SSelectionScrollBoxWidget> MyRoadSelectionScrollBox;
};
