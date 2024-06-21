// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingEditorWidget.h"
#include "ArchVizExplorerController.h"
#include <Kismet/GameplayStatics.h>

void UBuildingEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (NumOfSegments) {
		NumOfSegments->OnValueChanged.AddDynamic(this, &UBuildingEditorWidget::OnNumSegmentsChanged);
	}
	if (DestroyComponent) {
		DestroyComponent->OnClicked.AddDynamic(this, &UBuildingEditorWidget::OnDestroyClicked);
	}
}

void UBuildingEditorWidget::OnNumSegmentsChanged(float Number)
{
	AArchVizExplorerController* Controller = Cast<AArchVizExplorerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (Controller)
	{
		Controller->SetNumberOfSegments(static_cast<int32>(Number));
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Sclae"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("No Controller"));

	}
}

void UBuildingEditorWidget::OnDestroyClicked() {
	AArchVizExplorerController* Controller = Cast<AArchVizExplorerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (Controller)
	{
		Controller->OnDestroyCustomBuidling();
	}
}