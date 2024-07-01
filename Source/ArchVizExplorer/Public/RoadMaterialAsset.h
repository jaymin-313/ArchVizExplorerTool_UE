// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoadMaterialAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType) 
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString MaterialName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* MaterialImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* MaterialReference;
};
USTRUCT(BlueprintType) 
struct FBuildingMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString BuildingMaterialName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* BuildingMaterialImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* BuildingMaterialReference;
};
USTRUCT(BlueprintType)
struct FBuildingTypeData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FString BuildingTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UTexture2D* BuildingTypeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	UStaticMesh* BuildingTypeReference;

};

USTRUCT(BlueprintType)
struct FDoorMeshTypeData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Door")
	FString DoorTypeName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Door")
	UTexture2D* DoorTypeImage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Door")
	UStaticMesh* DoorTypeReference;
};
USTRUCT(BlueprintType)
struct FInteriorData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	FString InteriorTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UTexture2D* InteriorTypeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UStaticMesh* InteriorTypeReference;

};
USTRUCT(BlueprintType)
struct FWallInteriorData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	FString WallInteriorTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UTexture2D* WallInteriorTypeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UStaticMesh* WallInteriorTypeReference;

};
USTRUCT(BlueprintType)
struct FCeilingInteriorData {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	FString CeilingInteriorTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UTexture2D* CeilingInteriorTypeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interior")
	UStaticMesh* CeilingInteriorTypeReference;

};
UCLASS()
class ARCHVIZEXPLORER_API URoadMaterialAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Materials")
	TArray<FMaterialData> Materials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FBuildingTypeData> BuildingTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FDoorMeshTypeData> DoorTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FBuildingMaterialData> BuildingMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FInteriorData> InteriorTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FWallInteriorData> WallInteriorTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FCeilingInteriorData> CeilingInteriorTypes;

};
