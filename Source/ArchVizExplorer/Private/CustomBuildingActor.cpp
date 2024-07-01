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

UStaticMesh* ACustomBuildingActor::GetStaticMesh()
{
	if (BuildingMeshComponent) {
		return BuildingMeshComponent->GetStaticMesh();
	}
	return nullptr;
}

void ACustomBuildingActor::SetMeshXScale(int32 Number)
{
	FVector OldScale = BuildingMeshComponent->GetRelativeScale3D();
	OldScale.X = Number;
	BuildingMeshComponent->SetRelativeScale3D(OldScale);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Scale Set"));
}
void ACustomBuildingActor::SetMeshYScale(int32 Number)
{
	FVector OldScale = BuildingMeshComponent->GetRelativeScale3D();
	OldScale.Y = Number;
	BuildingMeshComponent->SetRelativeScale3D(OldScale);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Scale Set"));
}
void ACustomBuildingActor::SetMeshScale(int32 Number)
{
	FVector OldScale = BuildingMeshComponent->GetRelativeScale3D();
	OldScale.X = Number;
	BuildingMeshComponent->SetRelativeScale3D(OldScale);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Scale Set"));
}

int32 ACustomBuildingActor::GetMeshScale()
{
	FVector OldScale = BuildingMeshComponent->GetRelativeScale3D();
	int32 Number = OldScale.X;
	return Number;
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
	}
	else if(InString.Equals( FString{ "Floor" })) {
		BuildType = ECustomBuildingType::Floor;
	}
	else if (InString.Equals(FString{ "Door" })){
		BuildType = ECustomBuildingType::Door;
	}
}

void ACustomBuildingActor::SetMaterial(UMaterialInterface* InMaterial)
{
	BuildingMeshComponent->SetMaterial(0, InMaterial);
	//BuildingMeshComponent->SetMaterial(2, InMaterial); // For Door

	
	
}
//void ACustomBuildingActor::SetFloorDetails(int32 CurrentFloor)
//{
//	BuildingFloorDetails = CurrentFloor;
//}
//
//int32 ACustomBuildingActor::GetFloorDetails()
//{
//	return BuildingFloorDetails;
//}
UMaterialInterface* ACustomBuildingActor::GetMaterial()
{
	if (BuildingMeshComponent) {
		return BuildingMeshComponent->GetMaterial(0);
	}
	return nullptr;
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

