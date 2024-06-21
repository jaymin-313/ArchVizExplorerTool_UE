// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBuildingData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Building")
	FString BuildingTypeName;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Building")
	UTexture2D* BuildingTypeImage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Building")
	UStaticMesh* BuildingTypeReference;

};
UCLASS()
class ARCHVIZEXPLORER_API UBuildingDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FBuildingData> BuildingTypes;
};
