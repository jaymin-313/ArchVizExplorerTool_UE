// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizExplorerController.h"
#include "Engine/DataTable.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>


void AArchVizExplorerController::BeginPlay()
{
	Super::BeginPlay();

	if (ArchVizWidgetClass) {
		ArchVizWidgetInstance = CreateWidget<UArchWizExplorerWidget>(this, ArchVizWidgetClass);
		RoadWidgetInstance = CreateWidget<URoadEditorWidget>(this, RoadWidgetClass);
		MaterialWidgetInstance = CreateWidget<UMaterialEditorWidget>(this, MaterialWidgetClass);
		BuildingWidgetInstance = CreateWidget<UBuildingEditorWidget>(this, BuildingWidgetClass);
		InteriorWidgetInstance = CreateWidget<UInteriorEditorWidget>(this, InteriorWidgetClass);

		if (ArchVizWidgetInstance && RoadWidgetInstance && MaterialWidgetInstance && BuildingWidgetClass && InteriorWidgetClass) {
			ArchVizWidgetInstance->AddToViewport();
			ArchVizWidgetInstance->CanvasPanel->SetVisibility(ESlateVisibility::Visible);
			MaterialWidgetInstance->MaterialScrollBox->MaterialSelected.BindUObject(this, &AArchVizExplorerController::ApplyMaterial);
			MaterialWidgetInstance->BuildingMaterialBox->BuildingMaterialSelected.BindUObject(this, &AArchVizExplorerController::ApplyBuildingMaterial);
			MaterialWidgetInstance->DoorMeshTypes->DoorSelected.BindUObject(this, &AArchVizExplorerController::ChangeDoorMesh);
			BuildingWidgetInstance->BuildingScrollBox->BuildingSelected.BindUObject(this, &AArchVizExplorerController::SelectType);
			InteriorWidgetInstance->InteriorScrollBox->InteriorSelected.BindUObject(this, &AArchVizExplorerController::PlaceInterior);
			InteriorWidgetInstance->WallInteriorScrollBox->WallInteriorSelected.BindUObject(this, &AArchVizExplorerController::PlaceWallInterior);
			InteriorWidgetInstance->CeilingInteriorScrollBox->CeilingInteriorSelected.BindUObject(this, &AArchVizExplorerController::PlaceCeilingInterior);
			ArchVizWidgetInstance->SetPlayerController(this);
			RoadWidgetInstance->SetPlayerController(this);
			BuildingWidgetInstance->SetPlayerController(this);
			InteriorWidgetInstance->SetPlayerController(this);
			bShowMouseCursor = true;

		}

	}
	TArray<FString> Filenames;
	FString DirectoryPath = FPaths::ProjectSavedDir() / TEXT("SaveGames");

	RetrieveFilenamesFromDirectory(DirectoryPath, Filenames);

	if (ArchVizWidgetInstance) {
		ArchVizWidgetInstance->PopulateComboBox(Filenames);
	}
}



void AArchVizExplorerController::Tick(float DeltaTime)
{
	if (CustomBuildingInstance && bIsBuildingMappingContext) {
		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, CustomBuildingInstance);
		FVector WorldLocation, WorldDirection;
		DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility, TraceParams);
		FVector HitLocation = HitResult.ImpactPoint;
		if (HitResult.IsValidBlockingHit())
		{
			if (CustomBuildingInstance->GetActorType() == "Roof") {

				HitResult.Location.Z = i + 420.0f;
			}
			else {
				HitLocation.Z = i + 0.0f;
			}
			CustomBuildingInstance->SetActorLocation(HitResult.Location);
			SnapActor(10.0f);
		}
	}


}
void AArchVizExplorerController::ApplyMaterial(const FMaterialData& MaterialData) {
	if (ProceduralRoadActor) {
		ProceduralRoadActor->SetMaterial(MaterialData.MaterialReference);
	}
}
void AArchVizExplorerController::SelectType(const FBuildingTypeData& BuildingData) {
	DestroyUnplacedActor();
	SpawnCustomBuildingActor();
	CustomBuildingInstance->SetStaticMesh(BuildingData.BuildingTypeReference);
	if (BuildingData.BuildingTypeName == "Wall" || BuildingData.BuildingTypeName == "Window Wall" || BuildingData.BuildingTypeName == "Door Wall" || BuildingData.BuildingTypeName == "Wall For Door") {
		CustomBuildingInstance->SetActorType(ECustomBuildingType::Wall);
		BuildingWidgetInstance->SegmentsBorder->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->NumOfSegments->SetValue(1.0);
		BuildingWidgetInstance->TwoDimensionBorder->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (BuildingData.BuildingTypeName == "Floor") {
		CustomBuildingInstance->SetActorType(ECustomBuildingType::Floor);
		BuildingWidgetInstance->SegmentsBorder->SetVisibility(ESlateVisibility::Hidden);
		BuildingWidgetInstance->NumOfRows->SetValue(1.0);
		BuildingWidgetInstance->NumOfColumns->SetValue(1.0);
		BuildingWidgetInstance->TwoDimensionBorder->SetVisibility(ESlateVisibility::Visible);
	}
	else if (BuildingData.BuildingTypeName == "Roof") {
		CustomBuildingInstance->SetActorType(ECustomBuildingType::Roof);
		BuildingWidgetInstance->NumOfRows->SetValue(1.0);
		BuildingWidgetInstance->NumOfColumns->SetValue(1.0);
		BuildingWidgetInstance->TwoDimensionBorder->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->SegmentsBorder->SetVisibility(ESlateVisibility::Hidden);

	}
	else if (BuildingData.BuildingTypeName == "Door") {
		CustomBuildingInstance->SetActorType(ECustomBuildingType::Door);
		BuildingWidgetInstance->SegmentsBorder->SetVisibility(ESlateVisibility::Hidden);
		BuildingWidgetInstance->TwoDimensionBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AArchVizExplorerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	RoadMapping = NewObject<UInputMappingContext>(this);
	MaterialMapping = NewObject<UInputMappingContext>(this);
	BuildingMapping = NewObject<UInputMappingContext>(this);
	InteriorMapping = NewObject<UInputMappingContext>(this);

	LeftClickAction = NewObject<UInputAction>(this);
	LeftClickAction->ValueType = EInputActionValueType::Boolean;
	RoadMapping->MapKey(LeftClickAction, EKeys::LeftMouseButton);

	M_LeftClickAction = NewObject<UInputAction>(this);
	M_LeftClickAction->ValueType = EInputActionValueType::Boolean;
	MaterialMapping->MapKey(M_LeftClickAction, EKeys::LeftMouseButton);

	B_LeftClickAction = NewObject<UInputAction>(this);
	B_LeftClickAction->ValueType = EInputActionValueType::Boolean;
	BuildingMapping->MapKey(B_LeftClickAction, EKeys::LeftMouseButton);
	B_RightClickAction = NewObject<UInputAction>(this);
	B_RightClickAction->ValueType = EInputActionValueType::Boolean;
	BuildingMapping->MapKey(B_RightClickAction, EKeys::RightMouseButton);
	RotateAction = NewObject<UInputAction>(this);
	RotateAction->ValueType = EInputActionValueType::Boolean;
	BuildingMapping->MapKey(RotateAction, EKeys::R);

	I_LeftClickAction = NewObject<UInputAction>(this);
	I_LeftClickAction->ValueType = EInputActionValueType::Boolean;
	InteriorMapping->MapKey(I_LeftClickAction, EKeys::LeftMouseButton);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (EIC) {
		EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AArchVizExplorerController::OnLeftMouseClick);
		EIC->BindAction(M_LeftClickAction, ETriggerEvent::Completed, this, &AArchVizExplorerController::M_LeftMouseClick);
		EIC->BindAction(B_LeftClickAction, ETriggerEvent::Completed, this, &AArchVizExplorerController::B_LeftMouseClick);
		EIC->BindAction(B_RightClickAction, ETriggerEvent::Completed, this, &AArchVizExplorerController::B_RightMouseClick);
		EIC->BindAction(RotateAction, ETriggerEvent::Completed, this, &AArchVizExplorerController::RotateCustomBuilding);
		EIC->BindAction(I_LeftClickAction, ETriggerEvent::Completed, this, &AArchVizExplorerController::I_LeftMouseClick);
	}
	LocalPlayer = GetLocalPlayer();
	Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->ClearAllMappings();
}

void AArchVizExplorerController::OnRoadWidthChanged(float Width)
{
	if (ProceduralRoadActor) {
		FVector Scale3D = ProceduralRoadActor->GetActorRelativeScale3D();
		Scale3D.Y = Width / 100;
		ProceduralRoadActor->SetActorRelativeScale3D(Scale3D);
	}
}
void AArchVizExplorerController::OnRoadLocationXChanged(float InLocation) {
	if (ProceduralRoadActor) {
		FVector OldLocation = ProceduralRoadActor->GetActorLocation();
		if (OldLocation.X != InLocation) {
			OldLocation.X = InLocation;
			ProceduralRoadActor->SetActorLocation(OldLocation);
		}
	}
}
void AArchVizExplorerController::OnRoadLocationYChanged(float InLocation) {
	if (ProceduralRoadActor) {
		FVector OldLocation = ProceduralRoadActor->GetActorLocation();
		if (OldLocation.Y != InLocation) {
			OldLocation.Y = InLocation;
			ProceduralRoadActor->SetActorLocation(OldLocation);
		}
	}
}

void AArchVizExplorerController::ShowGuide() {
	if (!bGuideVisible) {
		ArchVizWidgetInstance->GuideSwitcher->SetVisibility(ESlateVisibility::Visible);
		ArchVizWidgetInstance->GuideText->SetText(FText::FromString("Hide Guide"));
		bGuideVisible = true;
	}
	else {
		ArchVizWidgetInstance->GuideSwitcher->SetVisibility(ESlateVisibility::Hidden);
		ArchVizWidgetInstance->GuideText->SetText(FText::FromString("Show Guide"));
		bGuideVisible = false;
	}
}
void AArchVizExplorerController::OnModeChanged(FString InSelectedItem)
{
	if (Subsystem) {
		Subsystem->ClearAllMappings();
		if (CustomBuildingInstance && !bIsPlaced) {
			CustomBuildingInstance->Destroy();
			CustomBuildingInstance = nullptr;
		}
		if (CustomBuildingInstance) {
			CustomBuildingInstance = nullptr;
			//CustomBuildingInstance->Destroy();
		}
		if (ProceduralRoadActor) {
			ProceduralRoadActor = nullptr;

		}
		bIsBuildingMappingContext = false;
		ClearViewportWidgets();
		if (InSelectedItem == "View Mode") {
			ShowMessage(FString{ "Changed To View Mode" });
			ArchVizWidgetInstance->GuideSwitcher->SetActiveWidgetIndex(4);
		}
		else if (InSelectedItem == "Road Construction") {
			Subsystem->AddMappingContext(RoadMapping, 0);
			if (RoadWidgetInstance) {
				RoadWidgetInstance->AddToViewport();
				ArchVizWidgetInstance->GuideSwitcher->SetActiveWidgetIndex(0);
				ShowMessage(FString{ "Changed To Road Mode" });
			}
		}
		else if (InSelectedItem == "Building Construction") {
			Subsystem->AddMappingContext(BuildingMapping, 0);
			bIsBuildingMappingContext = true;
			if (BuildingWidgetInstance) {
				BuildingWidgetInstance->AddToViewport();
				ArchVizWidgetInstance->GuideSwitcher->SetActiveWidgetIndex(1);

				ShowMessage(FString{ "Changed To Building Mode" });
			}
		}
		else if (InSelectedItem == "Interior Design") {
			Subsystem->AddMappingContext(InteriorMapping, 0);
			if (ArchVizWidgetInstance) {
				InteriorWidgetInstance->AddToViewport();
				ArchVizWidgetInstance->GuideSwitcher->SetActiveWidgetIndex(2);

				ShowMessage(FString{ "Changed To Interior Mode" });
				SpawnCustomBuildingActor();
			}

		}
		else if (InSelectedItem == "Material Selection") {
			Subsystem->AddMappingContext(MaterialMapping, 0);
			if (MaterialWidgetInstance) {
				MaterialWidgetInstance->AddToViewport();
				ArchVizWidgetInstance->GuideSwitcher->SetActiveWidgetIndex(3);
				ShowMessage(FString{ "Changed To Material Mode" });
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Invalid Mode"));
		}

	}
}
void AArchVizExplorerController::ShowMessage(FString Message) {
	ArchVizWidgetInstance->MessageBorder->SetVisibility(ESlateVisibility::Visible);
	ArchVizWidgetInstance->MessageTextBlock->SetText(FText::FromString(Message));
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AArchVizExplorerController::HideInstructionText, 5, false);
}
void AArchVizExplorerController::HideInstructionText() {
	ArchVizWidgetInstance->MessageBorder->SetVisibility(ESlateVisibility::Hidden);

}
void AArchVizExplorerController::ClearViewportWidgets() {
	if (RoadWidgetInstance && MaterialWidgetInstance && BuildingWidgetInstance && InteriorWidgetInstance) {
		RoadWidgetInstance->RemoveFromParent();
		MaterialWidgetInstance->RemoveFromParent();
		BuildingWidgetInstance->RemoveFromParent();
		InteriorWidgetInstance->RemoveFromParent();
	}
}

void AArchVizExplorerController::OnDestroyRoad() {
	if (ProceduralRoadActor) {
		ProceduralRoadActor->Destroy();
		ProceduralRoadActor = nullptr;
		ShowMessage(FString{ "Road Destroyed" });
	}
}
void AArchVizExplorerController::OnDestroyInterior()
{
	if (CustomInteriorInstance) {
		CustomInteriorInstance->Destroy();
		CustomInteriorInstance = nullptr;
		ShowMessage(FString{ "Interior Destroyed" });
	}
}
void AArchVizExplorerController::OnRotateInterior()
{
	if (CustomInteriorInstance) {
		FRotator Rotation = CustomInteriorInstance->GetActorRotation();
		Rotation.Yaw += 90.0F;
		CustomInteriorInstance->SetActorRotation(Rotation);
		ShowMessage(FString{ "Interior Rotated" });
	}
}
void AArchVizExplorerController::OnDestroyCustomBuidling()
{
	if (CustomBuildingInstance) {
		CustomBuildingInstance->Destroy();
		CustomBuildingInstance = nullptr;
		ShowMessage(FString{ "Current Building Actor Destroyed" });
	}
}
void AArchVizExplorerController::OnLeftMouseClick()
{
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
	FVector HitLocation = HitResult.ImpactPoint;
	ACustomBuildingActor* HitBuildingActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
	if (HitBuildingActor) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("This is Building Actor so No Road can be created"));
		ShowMessage(FString{ "This Is Building Actor So No Road Can Be Created" });
		return;
	}
	ACustomInteriorActor* HitInteriorActor = Cast<ACustomInteriorActor>(HitResult.GetActor());
	if (HitInteriorActor) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("This is Interior Actor so No Road can be created"));
		ShowMessage(FString{ "This Is Interior Actor So No Road Can Be Created" });
		return;
	}
	AProcRoadActor* HitRoadActor = Cast<AProcRoadActor>(HitResult.GetActor());
	if (HitRoadActor) {
		ProceduralRoadActor = HitRoadActor;
		double Width = ProceduralRoadActor->GetActorRelativeScale3D().Y;
		FVector RoadLocation = ProceduralRoadActor->GetActorLocation();
		/*ProceduralRoadActor->GetProceduralMeshComponent()->SetRenderCustomDepth(true);
		ProceduralRoadActor->GetProceduralMeshComponent()->SetCustomDepthStencilValue(2);*/
		if (RoadWidgetInstance)
		{
			RoadWidgetInstance->RoadWidth->SetValue(Width * 100);
			RoadWidgetInstance->RoadLocationX->SetValue(RoadLocation.X);
			RoadWidgetInstance->RoadLocationY->SetValue(RoadLocation.Y);
		}
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (!bIsFirstPointSelected) {
			FirstPoint = HitLocation;
			bIsFirstPointSelected = true;
		}
		else {
			SecondPoint = HitLocation;
			bIsFirstPointSelected = false;
			FVector MiddlePoint = (FirstPoint + SecondPoint) / 2;
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FirstPoint, SecondPoint);
			float Length = FVector::Dist(SecondPoint, FirstPoint);
			ProceduralRoadActor = GetWorld()->SpawnActor<AProcRoadActor>(ProceduralRoadActor->StaticClass(), MiddlePoint, Rotation, SpawnParams);
			//float  Length, Width, Height;
			if (ProceduralRoadActor) {
				ProceduralRoadActor->GenerateRoadMesh(Length, 100, 3);
				ShowMessage(FString{ "Road Created" });
			}
			if (RoadWidgetInstance) {
				FVector RoadLocation = ProceduralRoadActor->GetActorLocation();

				RoadWidgetInstance->RoadWidth->SetValue(100);
				RoadWidgetInstance->RoadLocationX->SetValue(RoadLocation.X);
				RoadWidgetInstance->RoadLocationY->SetValue(RoadLocation.Y);
			}
		}
	}

}
void AArchVizExplorerController::M_LeftMouseClick()
{

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("FRom material left cllicked"));
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
	FVector HitLocation = HitResult.ImpactPoint;
	ACustomInteriorActor* HitInteriorActor = Cast<ACustomInteriorActor>(HitResult.GetActor());
	if (HitInteriorActor) {
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Interior has material no need to override"));
		//HitInteriorActor->InteriorMeshComponent->SetRenderCustomDepth(true);
		//HitInteriorActor->InteriorMeshComponent->SetCustomDepthStencilValue(2);
		MaterialWidgetInstance->DoorMeshTypes->SetVisibility(ESlateVisibility::Hidden);
		MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialWidgetInstance->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
		return;

	}

	AProcRoadActor* HitRoadActor = Cast<AProcRoadActor>(HitResult.GetActor());
	if (HitRoadActor) {
		MaterialWidgetInstance->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
		ProceduralRoadActor = HitRoadActor;
	}
	else {
		MaterialWidgetInstance->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
	}
	ACustomBuildingActor* HitBuilidngActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
	if (HitBuilidngActor && !HitBuilidngActor->GetActorType().Equals(FString{ "Door" })) {
		MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Visible);
		MaterialWidgetInstance->DoorMeshTypes->SetVisibility(ESlateVisibility::Hidden);

		CustomBuildingInstance = HitBuilidngActor;

		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("Actor Selwected"));
	}
	else if (HitBuilidngActor && HitBuilidngActor->GetActorType().Equals(FString{ "Door" })) {
		CustomBuildingInstance = HitBuilidngActor;
		MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Hidden);
		MaterialWidgetInstance->DoorMeshTypes->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		MaterialWidgetInstance->DoorMeshTypes->SetVisibility(ESlateVisibility::Hidden);
		MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Hidden);

	}

}
void AArchVizExplorerController::ApplyBuildingMaterial(const FBuildingMaterialData& BuildingMaterialData) {
	if (CustomBuildingInstance) {
		CustomBuildingInstance->SetMaterial(BuildingMaterialData.BuildingMaterialReference);
		ShowMessage(FString{ "Matrial Applied" });
	}
}

void AArchVizExplorerController::ChangeDoorMesh(const FDoorMeshTypeData& DoorData) {
	if (CustomBuildingInstance) {
		CustomBuildingInstance->SetStaticMesh(DoorData.DoorTypeReference);
		ShowMessage(FString{ "Door Mesh Changed" });
	}
}
void AArchVizExplorerController::B_LeftMouseClick() {
	BuildingWidgetInstance->DestroyComponent->SetVisibility(ESlateVisibility::Hidden);
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
	FVector HitLocation = HitResult.ImpactPoint;
	if (HitResult.IsValidBlockingHit() && CustomBuildingInstance) {
		if (CustomBuildingInstance->GetActorType() == "Roof") {
			HitResult.Location.Z = i + 420.0f;
		}
		else {
			HitResult.Location.Z = i + 0.0f;
		}
		CustomBuildingInstance->SetActorLocation(HitResult.Location);
		SnapActor(10.0f);
		bIsPlaced = true;
		CustomBuildingInstance = nullptr;
	}
}
void AArchVizExplorerController::B_RightMouseClick() {
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);

	ACustomBuildingActor* HitActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
	if (HitActor && HitActor != CustomBuildingInstance) {
		BuildingWidgetInstance->DestroyComponent->SetVisibility(ESlateVisibility::Visible);
		CustomBuildingInstance = HitActor;
	}
	else {
		BuildingWidgetInstance->DestroyComponent->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AArchVizExplorerController::FirstFloorSelected() {
	FString CurrentValue = (ArchVizWidgetInstance->CurrentFloor->GetText().ToString());
	CurrentFloor = FCString::Atoi(*CurrentValue);
	CurrentFloor++;
	FString NewValue = FString::FromInt(CurrentFloor);
	ArchVizWidgetInstance->CurrentFloor->SetText(FText::FromString(NewValue));
	ShowMessage(FString{ "Floor Incremented" });

	i = CurrentFloor * 440.0f;
}
void AArchVizExplorerController::GroundFloorSelected() {
	FString CurrentValue = (ArchVizWidgetInstance->CurrentFloor->GetText().ToString());
	CurrentFloor = FCString::Atoi(*CurrentValue);
	CurrentFloor--;
	FString NewValue = FString::FromInt(CurrentFloor);
	ArchVizWidgetInstance->CurrentFloor->SetText(FText::FromString(NewValue));
	ShowMessage(FString{ "Floor Decremented" });
	i = CurrentFloor * 440.0f;
}
void AArchVizExplorerController::SpawnCustomBuildingActor() {

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CustomBuildingInstance = GetWorld()->SpawnActor<ACustomBuildingActor>(CustomBuildingClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	bIsPlaced = false;
}
void AArchVizExplorerController::RotateCustomBuilding() {
	if (CustomBuildingInstance) {

		FRotator NewRotation = FRotator(0, 90, 0);
		CustomBuildingInstance->SetActorRelativeRotation(CustomBuildingInstance->GetActorRotation() + NewRotation);
	}
}


void AArchVizExplorerController::SetNumberOfSegments(int32 Number) {
	if (CustomBuildingInstance) {
		NumberOfSegments = Number;
		CustomBuildingInstance->SetMeshScale(NumberOfSegments);
	}
}
void AArchVizExplorerController::SetNumberOfColumns(int32 Number) {
	if (CustomBuildingInstance) {
		CustomBuildingInstance->SetMeshXScale(Number);
	}
}
void AArchVizExplorerController::SetNumberOfRows(int32 Number) {
	if (CustomBuildingInstance) {
		CustomBuildingInstance->SetMeshYScale(Number);
	}
}

void AArchVizExplorerController::SnapActor(float SnapValue){
	if (CustomBuildingInstance) {
		auto CurrentLocation = CustomBuildingInstance->GetActorLocation();
		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;
		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;
		CustomBuildingInstance->SetActorLocation(CurrentLocation);
	}
}

void AArchVizExplorerController::DestroyUnplacedActor()
{
	if (CustomBuildingInstance)
	{
		CustomBuildingInstance->Destroy();
		CustomBuildingInstance = nullptr;
	}
}

void AArchVizExplorerController::I_LeftMouseClick()
{
	FHitResult HitResult;
	FVector WorldLocation, WorldDirection;
	DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
	FVector HitLocation = HitResult.ImpactPoint;
	ACustomBuildingActor* HitBuilidngActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
	if (HitBuilidngActor) {
		CustomBuildingInstance = HitBuilidngActor;
		bIsPlaced = true;
		if (CustomBuildingInstance->GetActorType().Equals(FString{ "Floor" })) {
			InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Visible);
			ShowMessage(FString{ "Floor Selected" });
			InteriorHitLocation = HitLocation;
		}
		else {

			InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else {
		InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
	}
	if (HitBuilidngActor) {
		CustomBuildingInstance = HitBuilidngActor;
		if (CustomBuildingInstance->GetActorType().Equals(FString{ "Wall" })) {
			ShowMessage(FString{ "Wall Selected" });
			InteriorWidgetInstance->WallInteriorScrollBox->SetVisibility(ESlateVisibility::Visible);
			InteriorHitLocation = HitLocation;
		}
		else {
			InteriorWidgetInstance->WallInteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);

		}
	}
	else {
		//DestroyUnplacedActor();
	}
	if (HitBuilidngActor) {
		CustomBuildingInstance = HitBuilidngActor;
		if (CustomBuildingInstance->GetActorType().Equals(FString{ "Roof" })) {
			InteriorWidgetInstance->CeilingInteriorScrollBox->SetVisibility(ESlateVisibility::Visible);
			ShowMessage(FString{ "Roof Selected" });
			InteriorHitLocation = HitLocation;
		}
		else {
			InteriorWidgetInstance->CeilingInteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);

		}
	}
	else {
		//DestroyUnplacedActor();

	}
	ACustomInteriorActor* HitInteriorActor = Cast<ACustomInteriorActor>(HitResult.GetActor());
	if (HitInteriorActor) {
		CustomInteriorInstance = HitInteriorActor;
		ShowMessage(FString{ "Interior Selected" });
		CustomBuildingInstance = GetCustomBuildingActor(CustomInteriorInstance->GetActorLocation(), GetActorExtentFromMesh(CustomInteriorInstance->GetStaticMesh()));
		InteriorWidgetInstance->DestroyInterior->SetVisibility(ESlateVisibility::Visible);
		InteriorWidgetInstance->RotateInterior->SetVisibility(ESlateVisibility::Visible);
		InteriorWidgetInstance->LocationBorder->SetVisibility(ESlateVisibility::Visible);
		if (CustomInteriorInstance->GetInteriorType().Equals(FString{ "Floor" }) || CustomInteriorInstance->GetInteriorType().Equals(FString{ "Roof" }))
			InteriorWidgetInstance->InteriorLocationZ->SetVisibility(ESlateVisibility::Hidden);
		else
			InteriorWidgetInstance->InteriorLocationZ->SetVisibility(ESlateVisibility::Visible);
		InteriorWidgetInstance->InteriorLocationX->SetValue(CustomInteriorInstance->GetActorLocation().X);
		InteriorWidgetInstance->InteriorLocationY->SetValue(CustomInteriorInstance->GetActorLocation().Y);

		InteriorWidgetInstance->InteriorLocationZ->SetValue(CustomInteriorInstance->GetActorLocation().Z);

	}
	else {
		InteriorWidgetInstance->DestroyInterior->SetVisibility(ESlateVisibility::Hidden);
		InteriorWidgetInstance->RotateInterior->SetVisibility(ESlateVisibility::Hidden);
		InteriorWidgetInstance->LocationBorder->SetVisibility(ESlateVisibility::Hidden);
	}

}
FVector AArchVizExplorerController::GetActorExtentFromMesh(UStaticMesh* Mesh) const {
	if (Mesh) {
		FVector Origin, BoxExtent;
		Mesh->GetBounds().GetBox().GetCenterAndExtents(Origin, BoxExtent);
		return BoxExtent;
	}
	return FVector(50.0f, 50.0f, 50.0f);
}

bool AArchVizExplorerController::CheckCollisionAtLocation(const FVector& Location, const FVector& ActorExtent, const TArray<FString>& IgnoredActorTypes, ACustomBuildingActor* ActorToIgnore)
{
	FString BuildingType{};
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(ActorExtent);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (ActorToIgnore) {
		BuildingType = ActorToIgnore->GetActorType();
	}

	bool bHasCollision = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		CollisionParams
	);

	if (bHasCollision) {
		for (const FOverlapResult& Overlap : Overlaps) {
			AActor* OverlappingActor = Overlap.GetActor();
			if (OverlappingActor) {
				ACustomBuildingActor* CustomBuildingActor = Cast<ACustomBuildingActor>(OverlappingActor);
				if (CustomBuildingActor) {
					FString ActorType = CustomBuildingActor->GetActorType();
					if (ActorType.Equals(BuildingType))
						continue;
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Checking FOr ActorType: %s."), *ActorType));
					if (IgnoredActorTypes.Contains(ActorType)) {
						ShowMessage(FString{ "Collision Detected" });
						//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Collision detected with %s."), *ActorType));
						return true;
					}
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Overlapping actor is not of type ACustomBuildingActor."));
				}
			}
		}
	}
	return false;
}
bool AArchVizExplorerController::IsInteriorOnBuildingActor(const FVector& Location, const FVector& ActorExtent, const TArray<FString>& IgnoredActorTypes, ACustomBuildingActor* ActorToIgnore)
{
	FString BuildingType{};
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(ActorExtent);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (ActorToIgnore) {
		BuildingType = ActorToIgnore->GetActorType();
	}

	bool bHasCollision = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		CollisionParams
	);

	if (bHasCollision) {
		for (const FOverlapResult& Overlap : Overlaps) {
			AActor* OverlappingActor = Overlap.GetActor();
			if (OverlappingActor) {
				ACustomBuildingActor* CustomBuildingActor = Cast<ACustomBuildingActor>(OverlappingActor);
				if (CustomBuildingActor) {
					FString ActorType = CustomBuildingActor->GetActorType();
					if (BuildingType.Equals(ActorType)) {
						return true;
					}
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Checking FOr ActorType: %s."), *ActorType));
				/*if (IgnoredActorTypes.Contains(ActorType)) {
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Collision detected with %s."), *ActorType));
					return true;
				}*/
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Overlapping actor is not of type ACustomBuildingActor."));
				}
			}
		}
	}

	return false;

}
ACustomBuildingActor* AArchVizExplorerController::GetCustomBuildingActor(const FVector& Location, const FVector& ActorExtent)
{
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(ActorExtent);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams CollisionParams;
	TArray<FString>IgnoredActorTypes = { "Wall","Floor","Roof", "Door" };

	bool bHasCollision = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		CollisionParams
	);

	if (bHasCollision) {
		for (const FOverlapResult& Overlap : Overlaps) {
			AActor* OverlappingActor = Overlap.GetActor();
			if (OverlappingActor) {
				ACustomBuildingActor* CustomBuildingActor = Cast<ACustomBuildingActor>(OverlappingActor);
				if (CustomBuildingActor && CustomInteriorInstance) {
					if (CustomBuildingActor->GetActorType().Equals(FString{ "Roof" }) && CustomInteriorInstance->GetInteriorType().Equals(FString{ "Floor" })) {
						bIsPlaced = true;
						continue;
					}
					if (CustomBuildingActor->GetActorType().Equals(FString{ "Floor" }) && CustomInteriorInstance->GetInteriorType().Equals(FString{ "Roof" })) {
						bIsPlaced = true;
						continue;
					}
					bIsPlaced = true;

					return CustomBuildingActor;
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("No actor"));
				}
			}
		}
	}
	return nullptr;
}
bool AArchVizExplorerController::CheckInteriorCollision(const FVector& Location, const FVector& ActorExtent, ACustomInteriorActor* ActorToIgnore)
{
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(ActorExtent);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (ActorToIgnore) {
		CollisionParams.AddIgnoredActor(ActorToIgnore);
	}

	bool bHasCollision = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Location,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		CollisionParams
	);

	if (bHasCollision) {
		for (const FOverlapResult& Overlap : Overlaps) {
			AActor* OverlappingActor = Overlap.GetActor();
			if (OverlappingActor) {
				ACustomInteriorActor* CustomInteriorActor = Cast<ACustomInteriorActor>(OverlappingActor);
				if (CustomInteriorActor) {
					ShowMessage(FString{ "Collision Detected With Interior" });

					return true;
				}
			}
		}
	}
	return false;
}
void AArchVizExplorerController::PlaceInterior(const FInteriorData& InteriorData) {
	FVector ActorExtent = GetActorExtentFromMesh(InteriorData.InteriorTypeReference);
	TArray<FString> IgnoredActorTypes = { "Wall", "Door" };

	if (CheckCollisionAtLocation(InteriorHitLocation, ActorExtent, IgnoredActorTypes) || CheckInteriorCollision(InteriorHitLocation, ActorExtent, nullptr)) {
		return;
	}


	if (CustomInteriorClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		CustomInteriorInstance = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorHitLocation, FRotator::ZeroRotator, SpawnParams);
		if (CustomInteriorInstance) {
			CustomInteriorInstance->SetStaticMesh(InteriorData.InteriorTypeReference);
			CustomInteriorInstance->SetInteriorType(FString{ "Floor" });
			ShowMessage(FString{ "Interior Placed" });
			CustomInteriorInstance = nullptr;
			InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Failed to spawn interior actor."));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("CustomInteriorClass is null. Cannot place interior."));
	}
}

void AArchVizExplorerController::PlaceWallInterior(const FWallInteriorData& WallInteriorData) {
	FVector ActorExtent = GetActorExtentFromMesh(WallInteriorData.WallInteriorTypeReference);
	TArray<FString> IgnoredActorTypes = { "Floor", "Roof", "Door", "Wall" };
	ACustomBuildingActor* SelectedWallActor = CustomBuildingInstance;

	if (CheckCollisionAtLocation(InteriorHitLocation, ActorExtent, IgnoredActorTypes, SelectedWallActor) || CheckInteriorCollision(InteriorHitLocation, ActorExtent, nullptr)) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("No collision detected, placing interior."));

	if (CustomInteriorClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		CustomInteriorInstance = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorHitLocation, FRotator::ZeroRotator, SpawnParams);
		if (CustomInteriorInstance) {
			CustomInteriorInstance->SetStaticMesh(WallInteriorData.WallInteriorTypeReference);
			CustomInteriorInstance->SetInteriorType(FString{ "Wall" });
			ShowMessage(FString{ "Interior Placed" });
			//CustomInteriorInstance = nullptr;
			InteriorWidgetInstance->WallInteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
			InteriorWidgetInstance->RotateInterior->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Failed to spawn interior actor."));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("CustomInteriorClass is null. Cannot place interior."));
	}
}

void AArchVizExplorerController::PlaceCeilingInterior(const FCeilingInteriorData& CeilingInteriorData) {
	FVector ActorExtent = GetActorExtentFromMesh(CeilingInteriorData.CeilingInteriorTypeReference);
	TArray<FString> IgnoredActorTypes = { "Wall", "Door" };
	ACustomBuildingActor* SelectedCeilingActor = CustomBuildingInstance;

	if (CheckCollisionAtLocation(InteriorHitLocation, ActorExtent, IgnoredActorTypes, SelectedCeilingActor) || CheckInteriorCollision(InteriorHitLocation, ActorExtent, nullptr)) {
		return;
	}

	if (CustomInteriorClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		CustomInteriorInstance = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorHitLocation, FRotator::ZeroRotator, SpawnParams);
		if (CustomInteriorInstance) {
			CustomInteriorInstance->SetStaticMesh(CeilingInteriorData.CeilingInteriorTypeReference);
			CustomInteriorInstance->SetInteriorType(FString{ "Roof" });
			ShowMessage(FString{ "Interior Placed" });
			CustomInteriorInstance = nullptr;
			InteriorWidgetInstance->CeilingInteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Failed to spawn interior actor."));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("CustomInteriorClass is null. Cannot place interior."));
	}
}
void AArchVizExplorerController::OnInteriorLocationXChanged(float InValue) {
	if (CustomInteriorInstance) {
		FVector OldLocation = CustomInteriorInstance->GetActorLocation();
		FVector NewLocation = OldLocation;
		NewLocation.X = InValue;

		UStaticMesh* Mesh = CustomInteriorInstance->GetStaticMesh();
		FVector ActorExtent = GetActorExtentFromMesh(Mesh);

		ACustomBuildingActor* CustomBuildingActor = CustomBuildingInstance;
		if (!CustomInteriorInstance->GetInteriorType().Equals(FString{ "Floor" })) {
			TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door" };
			if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor) || CheckInteriorCollision(NewLocation, ActorExtent, CustomInteriorInstance) || !IsInteriorOnBuildingActor(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
				ShowMessage(FString{ "Collision Detected At New Location" });
				CustomInteriorInstance->SetActorLocation(OldLocation);
				return;
			}

		}

		else {
			TArray<FString> IgnoredActorTypes = { "Wall", "Door","Floor" }; // removed roof from this to work with floor first couch
			if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor) || CheckInteriorCollision(NewLocation, ActorExtent, CustomInteriorInstance) || !IsInteriorOnBuildingActor(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
				ShowMessage(FString{ "Collision Detected At New Location" });
				CustomInteriorInstance->SetActorLocation(OldLocation);
				return;
			}
		}

		CustomInteriorInstance->SetActorLocation(NewLocation);
	}
}

void AArchVizExplorerController::OnInteriorLocationYChanged(float InValue) {
	if (CustomInteriorInstance) {
		FVector OldLocation = CustomInteriorInstance->GetActorLocation();
		FVector NewLocation = OldLocation;
		NewLocation.Y = InValue;
		UStaticMesh* Mesh = CustomInteriorInstance->GetStaticMesh();
		FVector ActorExtent = GetActorExtentFromMesh(Mesh);
		ACustomBuildingActor* CustomBuildingActor = CustomBuildingInstance;
		if (!CustomInteriorInstance->GetInteriorType().Equals(FString{ "Floor" })) {
			TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door" };
			if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor) || CheckInteriorCollision(NewLocation, ActorExtent, CustomInteriorInstance) || !IsInteriorOnBuildingActor(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
				ShowMessage(FString{ "Collision Detected At New Location" });
				CustomInteriorInstance->SetActorLocation(OldLocation);
				return;
			}

		}
		else {
			TArray<FString> IgnoredActorTypes = { "Wall", "Door","Floor" };
			if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor) || CheckInteriorCollision(NewLocation, ActorExtent, CustomInteriorInstance) || !IsInteriorOnBuildingActor(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
				ShowMessage(FString{ "Collision Detected At New Location" });
				CustomInteriorInstance->SetActorLocation(OldLocation);
				return;
			}
		}

		CustomInteriorInstance->SetActorLocation(NewLocation);
	}
}

void AArchVizExplorerController::OnInteriorLocationZChanged(float InValue) {
	if (CustomInteriorInstance) {
		FVector OldLocation = CustomInteriorInstance->GetActorLocation();
		FVector NewLocation = OldLocation;
		NewLocation.Z = InValue;

		UStaticMesh* Mesh = CustomInteriorInstance->GetStaticMesh();
		FVector ActorExtent = GetActorExtentFromMesh(Mesh);
		ACustomBuildingActor* CustomBuildingActor = CustomBuildingInstance;
		if (!CustomInteriorInstance->GetInteriorType().Equals(FString{ "Floor" })) {
			TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door" };
			if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor) || CheckInteriorCollision(NewLocation, ActorExtent, CustomInteriorInstance) || !CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes) || !IsInteriorOnBuildingActor(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
				ShowMessage(FString{ "Collision Detected At New Location" });
				CustomInteriorInstance->SetActorLocation(OldLocation);
				return;
			}

		}
		else {
			TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door","Floor" };
			if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor) || CheckInteriorCollision(NewLocation, ActorExtent, CustomInteriorInstance) || !CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes) || !IsInteriorOnBuildingActor(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
				ShowMessage(FString{ "Collision Detected At New Location" });
				CustomInteriorInstance->SetActorLocation(OldLocation);
				return;
			}
		}

		CustomInteriorInstance->SetActorLocation(NewLocation);
	}
}

//Save game

void AArchVizExplorerController::OnSaveGameClicked() {
	if (ArchVizWidgetInstance) {
		ShowMessage(FString{ "Enter New Project Name/Check For Same Loaded Project Name" });
		if (ArchVizWidgetInstance->SavedBuildingOptions->GetSelectedOption().Equals(FString{ "" })) {
			ArchVizWidgetInstance->NameOfBuilding->SetText(FText::FromString("Enter Name"));
		}
		else {
			ArchVizWidgetInstance->NameOfBuilding->SetText(FText::FromString(ArchVizWidgetInstance->SavedBuildingOptions->GetSelectedOption()));
		}
		ArchVizWidgetInstance->SaveBorder->SetVisibility(ESlateVisibility::Visible);
	}
}

void AArchVizExplorerController::OnLoadGameClicked() {
	if (ArchVizWidgetInstance) {
		ShowMessage(FString{ "Select Project From Above Given ComboBox To Load" });
		ArchVizWidgetInstance->SavedBuildingOptions->SetVisibility(ESlateVisibility::Visible);
	}
}
void AArchVizExplorerController::SaveGame(FString InName)
{
	UArchVizSaveGame* SaveGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveGame::StaticClass()));

	if (!SaveGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to create SaveGameInstance"));
		return;
	}


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProcRoadActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AProcRoadActor* ProcRoad = Cast<AProcRoadActor>(Actor);
		if (ProcRoad)
		{
			FRoadData RoadData;
			RoadData.Location = ProcRoad->GetActorLocation();
			RoadData.Rotation = ProcRoad->GetActorRotation();
			RoadData.Length = ProcRoad->GetLength();
			RoadData.Width = ProcRoad->GetWidth();
			RoadData.Height = ProcRoad->GetHeight();
			RoadData.RoadScale = ProcRoad->GetActorRelativeScale3D();
			RoadData.RoadMaterial = ProcRoad->GetMaterial();
			SaveGameInstance->ProceduralRoads.Add(RoadData);
		}
	}


	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomBuildingActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ACustomBuildingActor* CustomBuilding = Cast<ACustomBuildingActor>(Actor);
		if (CustomBuilding)
		{
			FCustomBuildingData BuildingData;
			BuildingData.Location = CustomBuilding->GetActorLocation();
			BuildingData.Rotation = CustomBuilding->GetActorRotation();
			BuildingData.Mesh = CustomBuilding->GetStaticMesh();
			BuildingData.Scale = CustomBuilding->GetActorScale3D();
			BuildingData.BuildingType = CustomBuilding->GetActorType();
			BuildingData.MaterialRef = CustomBuilding->GetMaterial();
			if (BuildingData.Location != FVector::ZeroVector) {
				SaveGameInstance->CustomBuildings.Add(BuildingData);
			}
			else {
				CustomBuilding->Destroy();
				CustomBuilding = nullptr;
			}
		}
	}


	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomInteriorActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ACustomInteriorActor* CustomInterior = Cast<ACustomInteriorActor>(Actor);
		if (CustomInterior)
		{
			FCustomInteriorData InteriorData;
			InteriorData.Location = CustomInterior->GetActorLocation();
			InteriorData.Rotation = CustomInterior->GetActorRotation();
			InteriorData.Mesh = CustomInterior->GetStaticMesh();
			InteriorData.InteriorType = CustomInterior->GetInteriorType();
			SaveGameInstance->CustomInteriors.Add(InteriorData);
		}
	}

	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, InName, 0))
	{
		ShowMessage(FString{ "Game Saved Successfully" });

	}
	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "MetaData", 0)) {
		SaveGameInstance->SavedBuildings.Add(InName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to save game"));
	}
	ArchVizWidgetInstance->SaveBorder->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizExplorerController::LoadGame(FString InName)
{

	UArchVizSaveGame* LoadGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(InName, 0));

	if (!LoadGameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to load game"));
		return;
	}
	for (auto it : BuildingNames) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, it);
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProcRoadActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		Actor->Destroy();
	}

	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomBuildingActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		Actor->Destroy();
	}

	FoundActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomInteriorActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		Actor->Destroy();
	}

	for (const FRoadData& RoadData : LoadGameInstance->ProceduralRoads)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AProcRoadActor* ProcActor = GetWorld()->SpawnActor<AProcRoadActor>(ProcRoadClass, RoadData.Location, RoadData.Rotation, SpawnParams);
		if (ProcActor)
		{
			ProcActor->GenerateRoadMesh(RoadData.Length, RoadData.Width, RoadData.Height);
			ProcActor->SetActorRelativeScale3D(RoadData.RoadScale);
			ProcActor->SetMaterial(RoadData.RoadMaterial);
		}
	}

	for (const FCustomBuildingData& BuildingData : LoadGameInstance->CustomBuildings)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		ACustomBuildingActor* CustomBuilding = GetWorld()->SpawnActor<ACustomBuildingActor>(CustomBuildingClass, BuildingData.Location, BuildingData.Rotation, SpawnParams);
		if (CustomBuilding)
		{
			CustomBuilding->SetStaticMesh(BuildingData.Mesh);
			CustomBuilding->SetActorScale3D(BuildingData.Scale);
			CustomBuilding->SetType(BuildingData.BuildingType);
			CustomBuilding->SetMaterial(BuildingData.MaterialRef);
		}
	}

	for (const FCustomInteriorData& InteriorData : LoadGameInstance->CustomInteriors)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		ACustomInteriorActor* CustomInterior = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorData.Location, InteriorData.Rotation, SpawnParams);
		if (CustomInterior)
		{
			CustomInterior->SetStaticMesh(InteriorData.Mesh);
			CustomInterior->SetInteriorType(InteriorData.InteriorType);
		}
	}

	ShowMessage(FString{ "Game Loaded Successfully" });

}

void AArchVizExplorerController::ClearSaveGameData()
{
	//// Load the save game object
	//UArchVizSaveGame* SaveGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveGame"), 0));
	////Templates.Empty();
	//if (SaveGameInstance)
	//{
	//    // Clear the data
	//    SaveGameInstance->ClearData();

	//    // Save the cleared data back to the slot
	//    if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySaveGame"), 0))
	//    {
	//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Save game data cleared successfully"));
	//    }
	//    else
	//    {
	//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to save cleared game data"));
	//    }
	//}
	//else
	//{
	//    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to load save game for clearing"));
	//    ArchVizWidgetInstance->SavedBuildingOptions->SetVisibility(ESlateVisibility::Hidden);
	//}
}
void AArchVizExplorerController::RetrieveFilenamesFromDirectory(const FString& DirectoryPath, TArray<FString>& OutFilenames)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.DirectoryExists(*DirectoryPath))
	{

		struct FFileVisitor : public IPlatformFile::FDirectoryVisitor
		{
			TArray<FString>& FileNames;
			FFileVisitor(TArray<FString>& InFileNames) : FileNames(InFileNames) {}

			virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
			{
				if (!bIsDirectory)
				{
					FileNames.Add(FPaths::GetBaseFilename(FilenameOrDirectory));
				}
				return true;
			}
		};

		FFileVisitor Visitor(OutFilenames);
		PlatformFile.IterateDirectory(*DirectoryPath, Visitor);
	}
}


