// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomBuildingActor.generated.h"
UENUM() 
enum class ECustomBuildingType : uint8 {
	Wall,
	Floor,
	Roof,
	Door
};
UCLASS()
class ARCHVIZEXPLORER_API ACustomBuildingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomBuildingActor();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Static Mesh")
	UStaticMesh* CustomStaticMesh;

	void SetStaticMesh(UStaticMesh* StaticMesh);
	UStaticMesh* GetStaticMesh();
	void SetMeshXScale(int32 Number);
	void SetMeshYScale(int32 Number);
	void SetMeshScale(int32 Number);
	int32 GetMeshScale();
	void SetActorType(ECustomBuildingType BuildType);
	void SetType(FString InString);
	void SetMaterial(UMaterialInterface* InMaterial);
	UMaterialInterface* GetMaterial();
	FString GetActorType();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building")
	UStaticMeshComponent* BuildingMeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TEnumAsByte<ECustomBuildingType> BuildType;

};
