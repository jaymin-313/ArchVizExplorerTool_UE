// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomInteriorActor.generated.h"
UENUM()
enum class ECustomInteriorType : uint8 {
	WallInterior,
	FloorInterior,
	RoofInterior,
};
UCLASS()
class ARCHVIZEXPLORER_API ACustomInteriorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomInteriorActor();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Interior")
	UStaticMesh* InteriorStaticMesh;
	UStaticMesh* GetStaticMesh();
	void SetStaticMesh(UStaticMesh* StaticMesh);
	void SetInteriorType(FString InInteriorType);
	FString GetInteriorType();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Interior")
	UStaticMeshComponent* InteriorMeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	TEnumAsByte<ECustomInteriorType> InteriorType;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
