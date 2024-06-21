// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBuildingActor.h"

// Sets default values
ACustomBuildingActor::ACustomBuildingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BuildingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuilidngComponent"));
	RootComponent = BuildingMeshComponent;
}

void ACustomBuildingActor::SetStaticMesh(UStaticMesh* StaticMesh)
{
	if (BuildingMeshComponent && StaticMesh) {
		BuildingMeshComponent->SetStaticMesh(StaticMesh);
	}
}

void ACustomBuildingActor::SetMeshScale(int32 Number)
{
	FVector OldScale = BuildingMeshComponent->GetRelativeScale3D();
	OldScale.X = Number;
	BuildingMeshComponent->SetRelativeScale3D(OldScale);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Scale Set"));
}

void ACustomBuildingActor::SetActorType(ECustomBuildingType InBuildType)
{
	BuildType = InBuildType;
}

void ACustomBuildingActor::SetType(FString InString) {
	if(InString.Equals( FString{ "Wall" })) {
		BuildType = ECustomBuildingType::Wall;
	}
	else if(InString.Equals( FString{ "Roof" })) {
		BuildType = ECustomBuildingType::Roof;
	}else if(InString.Equals( FString{ "Floor" })) {
		BuildType = ECustomBuildingType::Floor;
	}
	else {
		BuildType = ECustomBuildingType::Door;
	}
}

void ACustomBuildingActor::SetMaterial(UMaterialInterface* InMaterial)
{
	BuildingMeshComponent->SetMaterial(0, InMaterial);
	//BuildingMeshComponent->SetMaterial(2, InMaterial); // For Door

	
	
}

FString ACustomBuildingActor::GetActorType()
{
	if (BuildType == ECustomBuildingType::Wall) {
		return FString{ "Wall" };
	}
	else if (BuildType == ECustomBuildingType::Floor) {
		return FString{ "Floor" };
	}
	else if (BuildType == ECustomBuildingType::Roof) {
		return FString{ "Roof" };
	}
	else if (BuildType == ECustomBuildingType::Door) {
		return FString{ "Door" };
	}
	else {
		return FString{"InValid"};
	}
}

// Called when the game starts or when spawned
void ACustomBuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

