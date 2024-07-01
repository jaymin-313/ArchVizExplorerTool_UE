// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcRoadActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AProcRoadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcRoadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* RoadMaterial = LoadObject<UMaterialInterface>(this, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Megascans/Surfaces/Fine_American_Road_sjfnbeaa/MI_Fine_American_Road_sjfnbeaa_2K.MI_Fine_American_Road_sjfnbeaa_2K'"));

	UPROPERTY()
	float Length;
	UPROPERTY()
	float Width;
	UPROPERTY()
	float Height;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GenerateRoadMesh(float Length, float Width, float Height);
	UProceduralMeshComponent* GetProceduralMeshComponent();
	void SetProceduralMeshComponent(UProceduralMeshComponent* ProcComponent);
	UMaterialInterface* GetMaterial();
	void SetMaterial(UMaterialInterface* UMI);
	float GetLength();
	float GetWidth();
	float GetHeight();
};
