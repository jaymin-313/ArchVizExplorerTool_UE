// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcRoadActor.h"

// Sets default values
AProcRoadActor::AProcRoadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMesh;

    Length = 100.0f;
    Width = 100.0f;
    Height = 100.0f;
}

// Called when the game starts or when spawned
void AProcRoadActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProcRoadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProcRoadActor::GenerateRoadMesh(float InLength,float InWidth,float InHeight)
{

    Length = InLength;
    Width = InWidth;
    Height = InHeight;
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    // Vertices for the box
    Vertices = {
         FVector(-Length / 2, -Width / 2, 0),  FVector(-Length / 2, Width / 2, 0),  FVector(Length / 2, Width / 2, 0),  FVector(Length / 2, -Width / 2, 0), // -Z
         FVector(-Length / 2, -Width / 2, Height),  FVector(Length / 2, -Width / 2, Height),  FVector(Length / 2, Width / 2, Height),  FVector(-Length / 2, Width / 2, Height), // +Z
         FVector(-Length / 2, -Width / 2, 0),  FVector(Length / 2, -Width / 2, 0),  FVector(Length / 2, -Width / 2, Height),  FVector(-Length / 2, -Width / 2, Height), // -Y
         FVector(-Length / 2, Width / 2, 0),  FVector(-Length / 2, Width / 2, Height),  FVector(Length / 2, Width / 2, Height),  FVector(Length / 2, Width / 2, 0), // +Y
         FVector(-Length / 2, -Width / 2, 0),  FVector(-Length / 2, -Width / 2, Height),  FVector(-Length / 2, Width / 2, Height),  FVector(-Length / 2, Width / 2, 0), // -X
         FVector(Length / 2, -Width / 2, 0),  FVector(Length / 2, Width / 2, 0),  FVector(Length / 2, Width / 2, Height),  FVector(Length / 2, -Width / 2, Height) // +X
    };

    Triangles = {
        0, 2, 1, 0, 3, 2, // -Z
        4, 6, 5, 4, 7, 6, // +Z
        8, 10, 9, 8, 11, 10, // -Y
        12, 14, 13, 12, 15, 14, // +Y
        16, 18, 17, 16, 19, 18, // -X
        20, 22, 21, 20, 23, 22 // +X
    };


    Normals = {
        FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), // -Z
        FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), // +Z
        FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), // -Y
        FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), // +Y
        FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), // -X
        FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0) // +X
    };


    UVs = {
        FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // -Z
        FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1), // +Z
        FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), FVector2D(0, 0), // -Y
        FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // +Y
        FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1), FVector2D(0, 0), // -X
        FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)  // +X
    };

    Tangents.Init(FProcMeshTangent(1.f, 0.f, 0.f), 24);

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
    ProceduralMesh->SetMaterial(0, RoadMaterial);
}

UProceduralMeshComponent* AProcRoadActor::GetProceduralMeshComponent()
{
    if (ProceduralMesh) {
        return ProceduralMesh;
    }
    return nullptr;
}

void AProcRoadActor::SetProceduralMeshComponent(UProceduralMeshComponent* ProcComponent)
{
    if (ProcComponent) {
        ProceduralMesh = ProcComponent;
    }
}

UMaterialInterface* AProcRoadActor::GetMaterial()
{
    if (ProceduralMesh) {
        return ProceduralMesh->GetMaterial(0);
    }
    return nullptr;
}

void AProcRoadActor::SetMaterial(UMaterialInterface* UMI)
{
    ProceduralMesh->SetMaterial(0, UMI);
}

float AProcRoadActor::GetLength()
{
    return Length;
}float AProcRoadActor::GetWidth()
{
    return Width;
}float AProcRoadActor::GetHeight()
{
    return Height;
}
