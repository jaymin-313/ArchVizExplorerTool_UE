// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ProceduralMeshComponent.h"
#include "ArchVizSaveGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FRoadData {
    GENERATED_BODY()

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Rotation;

    UPROPERTY()
    float Length;

    UPROPERTY()
    float Width;

    UPROPERTY()
    float Height;

    UPROPERTY()
    FVector RoadScale;
    UPROPERTY()
    UMaterialInterface* RoadMaterial;
};
USTRUCT(BlueprintType)
struct FCustomBuildingData
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Rotation;

    UPROPERTY()
    UStaticMesh* Mesh;

    UPROPERTY()
    FString MeshPath;

    UPROPERTY()
    FVector Scale;

    UPROPERTY()
    FString BuildingType;

    UPROPERTY()
    UMaterialInterface* MaterialRef;
};

USTRUCT(BlueprintType)
struct FCustomInteriorData
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Rotation;

    UPROPERTY()
    UStaticMesh* Mesh;

    UPROPERTY()
    FString InteriorType;
    
};


//USTRUCT(BlueprintType)
//struct FTemplateData {
//    GENERATED_BODY()
//
//    UPROPERTY()
//    TArray<FCustomBuildingData> BuildingTemplates;
//
//    UPROPERTY()
//    TArray<FCustomInteriorData> InteriorTemplates;
//};



UCLASS()
class ARCHVIZEXPLORER_API UArchVizSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

    UPROPERTY()
    TArray<FRoadData> ProceduralRoads;
    UPROPERTY()
    TArray<FCustomBuildingData> CustomBuildings;

    UPROPERTY()
    TArray<FCustomInteriorData> CustomInteriors;

    /*UPROPERTY()
    TArray<FTemplateData> Templates;*/

    UPROPERTY()
    TArray <FString> SavedBuildings;

    void ClearData()
    {
        ProceduralRoads.Empty();
        CustomBuildings.Empty();
        CustomInteriors.Empty();
        //Templates.Empty();
    }
};
