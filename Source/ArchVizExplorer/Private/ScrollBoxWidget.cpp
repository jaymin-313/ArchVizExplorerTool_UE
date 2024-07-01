// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollBoxWidget.h"

TSharedRef<SWidget> UScrollBoxWidget::RebuildWidget()
{
	MyRoadSelectionScrollBox = SNew(SSelectionScrollBoxWidget).InMeshAsset(MeshAssetManger).InMeshType(AssetType);
	MyRoadSelectionScrollBox->OnRoadMaterialSelected.BindUObject(this, &UScrollBoxWidget::HandleMaterialSelection);
	MyRoadSelectionScrollBox->OnBuildingTypeSelected.BindUObject(this, &UScrollBoxWidget::HandleBuildingTypeSelection);
	MyRoadSelectionScrollBox->OnDoorMeshTypeSelected.BindUObject(this, &UScrollBoxWidget::HandleDoorTypeSelection);
	MyRoadSelectionScrollBox->OnInteriorTypeSelected.BindUObject(this, &UScrollBoxWidget::HandleInteriorTypeSelection);
	MyRoadSelectionScrollBox->OnWallInteriorTypeSelected.BindUObject(this, &UScrollBoxWidget::HandleWallInteriorTypeSelection);
	MyRoadSelectionScrollBox->OnCeilingInteriorTypeSelected.BindUObject(this, &UScrollBoxWidget::HandleCeilingInteriorTypeSelection);
	MyRoadSelectionScrollBox->OnBuildingMaterialSelected.BindUObject(this, &UScrollBoxWidget::HandleBuildingMaterialSelection);
	return MyRoadSelectionScrollBox.ToSharedRef();
}

void UScrollBoxWidget::HandleMaterialSelection(const FMaterialData& MaterialData)
{
	MaterialSelected.ExecuteIfBound(MaterialData);
}
void UScrollBoxWidget::HandleBuildingMaterialSelection(const FBuildingMaterialData& BuildingMaterialData)
{
	BuildingMaterialSelected.ExecuteIfBound(BuildingMaterialData);
}
void UScrollBoxWidget::HandleBuildingTypeSelection(const FBuildingTypeData& BuildingData)
{
	BuildingSelected.ExecuteIfBound(BuildingData);
}
void UScrollBoxWidget::HandleDoorTypeSelection(const FDoorMeshTypeData& DoorData)
{
	DoorSelected.ExecuteIfBound(DoorData);
}
void UScrollBoxWidget::HandleInteriorTypeSelection(const FInteriorData& InteriorData)
{
	InteriorSelected.ExecuteIfBound(InteriorData);
}
void UScrollBoxWidget::HandleWallInteriorTypeSelection(const FWallInteriorData& WallInteriorData) {
	WallInteriorSelected.ExecuteIfBound(WallInteriorData);
}
void UScrollBoxWidget::HandleCeilingInteriorTypeSelection(const FCeilingInteriorData& CeilingInteriorData) {
	CeilingInteriorSelected.ExecuteIfBound(CeilingInteriorData);
}
void UScrollBoxWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyRoadSelectionScrollBox.Reset();
}

const FText UScrollBoxWidget::GetPaletteCategory()
{
	return FText::FromString("Panel");
}


