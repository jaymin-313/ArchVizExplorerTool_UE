// Fill out your copyright notice in the Description page of Project Settings.


#include "SSelectionScrollBoxWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSelectionScrollBoxWidget::Construct(const FArguments& InArgs)
{
	FSlateBrush* BackgroundBrush = new FSlateBrush();
	BackgroundBrush->TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	ScrollBox = SNew(SScrollBox).Orientation(EOrientation::Orient_Horizontal);


	MeshAsset = InArgs._InMeshAsset;
	MeshType = InArgs._InMeshType;
	ChildSlot
		[
			SNew(SBorder)
				.BorderImage(BackgroundBrush)[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
						[
							SNew(STextBlock)
								.Text(GetAssetTypeName())
								.ColorAndOpacity(FColor::Blue)
								.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20))
						]
						+ SVerticalBox::Slot()[

							ScrollBox.ToSharedRef()
						]

				]
		];
	if (MeshType == EAssetType::Road)
		RefreshRoadMaterialThumbnails();
	else if (MeshType == EAssetType::BuildingType)
		RefreshBuildingTypeThumbnails();
	else if (MeshType == EAssetType::Interior)
		RefreshInteriorTypeThumbnails();
	else if (MeshType == EAssetType::BuildingMaterial)
		RefreshBuildingMaterialThumbnails();
	else if (MeshType == EAssetType::WallInterior)
		RefreshWallInteriorTypeThumbnails();
	else if (MeshType == EAssetType::CeilingInterior)
		RefreshCeilingInteriorTypeThumbnails();
	else if (MeshType == EAssetType::DoorMesh)
		RefreshDoorMeshThumbnails();
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSelectionScrollBoxWidget::RefreshRoadMaterialThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& MeshData : MeshAsset->Materials) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.MaterialImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.MaterialName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnRoadMaterialSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SSelectionScrollBoxWidget::RefreshBuildingTypeThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& BuildingData : MeshAsset->BuildingTypes) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(BuildingData.BuildingTypeImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(BuildingData.BuildingTypeName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, BuildingData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnBuildingTypeSelected.ExecuteIfBound(BuildingData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SSelectionScrollBoxWidget::RefreshDoorMeshThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& DoorData : MeshAsset->DoorTypes) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(DoorData.DoorTypeImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(DoorData.DoorTypeName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, DoorData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnDoorMeshTypeSelected.ExecuteIfBound(DoorData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SSelectionScrollBoxWidget::RefreshInteriorTypeThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& InteriorData : MeshAsset->InteriorTypes) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(InteriorData.InteriorTypeImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(InteriorData.InteriorTypeName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, InteriorData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnInteriorTypeSelected.ExecuteIfBound(InteriorData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SSelectionScrollBoxWidget::RefreshWallInteriorTypeThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& WallInteriorData : MeshAsset->WallInteriorTypes) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(WallInteriorData.WallInteriorTypeImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(WallInteriorData.WallInteriorTypeName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, WallInteriorData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnWallInteriorTypeSelected.ExecuteIfBound(WallInteriorData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SSelectionScrollBoxWidget::RefreshCeilingInteriorTypeThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& CeilingInteriorData : MeshAsset->CeilingInteriorTypes) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(CeilingInteriorData.CeilingInteriorTypeImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(CeilingInteriorData.CeilingInteriorTypeName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, CeilingInteriorData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnCeilingInteriorTypeSelected.ExecuteIfBound(CeilingInteriorData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SSelectionScrollBoxWidget::RefreshBuildingMaterialThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& BuilidngMaterialData : MeshAsset->BuildingMaterials) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(BuilidngMaterialData.BuildingMaterialImage);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(BuilidngMaterialData.BuildingMaterialName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, BuilidngMaterialData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnBuildingMaterialSelected.ExecuteIfBound(BuilidngMaterialData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

FText SSelectionScrollBoxWidget::GetAssetTypeName() const
{
	switch (MeshType)
	{
	case EAssetType::Road:
		return FText::FromString(TEXT("Road Materials")); // Adjust the string as needed
	case EAssetType::BuildingType:
		return FText::FromString(TEXT("Custom Building")); // Adjust the string as needed
	case EAssetType::Interior:
		return FText::FromString(TEXT("Interior")); // Adjust the string as needed
	case EAssetType::BuildingMaterial:
		return FText::FromString(TEXT("Builidng Materials")); // Adjust the string as needed
	case EAssetType::WallInterior:
		return FText::FromString(TEXT("Wall Interiors")); // Adjust the string as needed
	case EAssetType::CeilingInterior:
		return FText::FromString(TEXT("Ceiling Interiors")); // Adjust the string as needed
	case EAssetType::DoorMesh:
		return FText::FromString(TEXT("Door Types")); // Adjust the string as needed
	default:
		return FText::FromString(TEXT("Unknown")); // Default case for unknown types
	}
}
