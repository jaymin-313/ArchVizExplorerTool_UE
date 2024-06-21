// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomInteriorActor.h"

// Sets default values
ACustomInteriorActor::ACustomInteriorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteriorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior Mesh Component"));
	RootComponent = InteriorMeshComponent;
}

UStaticMesh* ACustomInteriorActor::GetStaticMesh()
{
	return InteriorMeshComponent->GetStaticMesh();
}

void ACustomInteriorActor::SetStaticMesh(UStaticMesh* StaticMesh)
{
	if (InteriorMeshComponent && StaticMesh )
		InteriorMeshComponent->SetStaticMesh(StaticMesh);
}

// Called when the game starts or when spawned
void ACustomInteriorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomInteriorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

