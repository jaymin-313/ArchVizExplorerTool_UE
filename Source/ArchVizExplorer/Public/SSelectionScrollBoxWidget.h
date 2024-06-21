// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoadMaterialAsset.h"
//#include "BuildingDataAsset.h"
#include "Widgets/SCompoundWidget.h"
#include <Widgets/Layout/SScrollBox.h>

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnRoadMaterialSelected, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FOnBuildingTypeSelected, const FBuildingTypeData&)
DECLARE_DELEGATE_OneParam(FOnInteriorTypeSelected, const FInteriorData&)
DECLARE_DELEGATE_OneParam(FOnWallInteriorTypeSelected, const FWallInteriorData&)
DECLARE_DELEGATE_OneParam(FOnCeilingInteriorTypeSelected, const FCeilingInteriorData&)
DECLARE_DELEGATE_OneParam(FOnBuildingMaterialSelected, const FBuildingMaterialData&)
UENUM()
enum class EAssetType {
	Road,
	BuildingType,
	Interior,
	BuildingMaterial,
	WallInterior,
	CeilingInterior
};
class ARCHVIZEXPLORER_API SSelectionScrollBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSelectionScrollBoxWidget)
	{}
		SLATE_ARGUMENT(TWeakObjectPtr<URoadMaterialAsset>, InMeshAsset)
	SLATE_ARGUMENT(EAssetType, InMeshType)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FOnRoadMaterialSelected OnRoadMaterialSelected;
	FOnBuildingTypeSelected OnBuildingTypeSelected;
	FOnInteriorTypeSelected OnInteriorTypeSelected;
	FOnWallInteriorTypeSelected OnWallInteriorTypeSelected;
	FOnCeilingInteriorTypeSelected OnCeilingInteriorTypeSelected;
	FOnBuildingMaterialSelected OnBuildingMaterialSelected;
private:
	void RefreshRoadMaterialThumbnails();
	void RefreshBuildingTypeThumbnails();
	void RefreshInteriorTypeThumbnails();
	void RefreshWallInteriorTypeThumbnails();
	void RefreshCeilingInteriorTypeThumbnails();
	void RefreshBuildingMaterialThumbnails();
	FText GetAssetTypeName() const;
	TWeakObjectPtr<URoadMaterialAsset> MeshAsset;
	EAssetType MeshType = EAssetType::Road;
	TSharedPtr<SScrollBox> ScrollBox;
};
