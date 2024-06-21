// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizExplorerController.h"

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
            RoadWidgetInstance->AddToViewport();
            MaterialWidgetInstance->MaterialScrollBox->MaterialSelected.BindUObject(this, &AArchVizExplorerController::ApplyMaterial);
            MaterialWidgetInstance->BuildingMaterialBox->BuildingMaterialSelected.BindUObject(this, &AArchVizExplorerController::ApplyBuildingMaterial);
            BuildingWidgetInstance->BuildingScrollBox->BuildingSelected.BindUObject(this, &AArchVizExplorerController::SelectType);
            InteriorWidgetInstance->InteriorScrollBox->InteriorSelected.BindUObject(this, &AArchVizExplorerController::PlaceInterior);
            InteriorWidgetInstance->WallInteriorScrollBox->WallInteriorSelected.BindUObject(this, &AArchVizExplorerController::PlaceWallInterior);
            InteriorWidgetInstance->CeilingInteriorScrollBox->CeilingInteriorSelected.BindUObject(this, &AArchVizExplorerController::PlaceCeilingInterior);
            ArchVizWidgetInstance->SetPlayerController(this);
            RoadWidgetInstance->SetPlayerController(this);
            bShowMouseCursor = true;
            
        }

    }
   
}



void AArchVizExplorerController::Tick(float DeltaTime)
{
    if (CustomBuildingInstance && bIsBuildingMappingContext) {
        FHitResult HitResult;
        FVector WorldLocation, WorldDirection;
        DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
        FVector HitLocation = HitResult.ImpactPoint;
        if (HitResult.IsValidBlockingHit())
        {
            if (CustomBuildingInstance->GetActorType() == "Roof") {

                HitResult.Location.Z = 420.0f;
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Roof"));
            }
            
            else {
                HitLocation.Z = 0.0f;
            }
            // Set the location of the SelectionAreaInstance
                //if(bMove)
                    CustomBuildingInstance->SetActorLocation(HitResult.Location);
                    SnapActor(20.0f);
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Actor changing location"));
        }
    }


}
void AArchVizExplorerController::ApplyMaterial(const FMaterialData& MaterialData) {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Applying Material"));
    if (ProceduralRoadActor) {
        ProceduralRoadActor->SetMaterial(MaterialData.MaterialReference);
    }
}
void AArchVizExplorerController::SelectType(const FBuildingTypeData& BuildingData) {
    DestroyUnplacedActor();
    SpawnCustomBuildingActor();
    CustomBuildingInstance->SetStaticMesh(BuildingData.BuildingTypeReference);
    if (BuildingData.BuildingTypeName == "Wall") {
        CustomBuildingInstance->SetActorType(ECustomBuildingType::Wall);
    }
    else if (BuildingData.BuildingTypeName == "Floor") {
        CustomBuildingInstance->SetActorType(ECustomBuildingType::Floor);
    } 
    else if (BuildingData.BuildingTypeName == "Roof") {
        CustomBuildingInstance->SetActorType(ECustomBuildingType::Roof);
    }
    else if (BuildingData.BuildingTypeName == "Door") {
        CustomBuildingInstance->SetActorType(ECustomBuildingType::Door);
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
    Subsystem->AddMappingContext(RoadMapping, 0);
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
void AArchVizExplorerController::OnModeChanged(FString InSelectedItem)
{
 
   if (Subsystem) {
        Subsystem->ClearAllMappings();
        bIsBuildingMappingContext = false;
        ClearViewportWidgets();
        if (InSelectedItem == "Road Construction") {
            Subsystem->AddMappingContext(RoadMapping, 0);
            if (RoadWidgetInstance) {
                RoadWidgetInstance->AddToViewport();
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Road Mode"));
            }
        }
        else if (InSelectedItem == "Building Construction") {
            Subsystem->AddMappingContext(BuildingMapping, 0);
            bIsBuildingMappingContext = true;
            if (BuildingWidgetInstance) {
                BuildingWidgetInstance->AddToViewport();
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Building Mode"));
            }
            /*if (CustomBuildingClass) {
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                CustomBuildingInstance = GetWorld()->SpawnActor<ACustomBuildingActor>(CustomBuildingClass, SpawnParams);
                bMove = true;
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Actor spawned"));
            }*/
        }
        else if (InSelectedItem == "Interior Design") {
            Subsystem->AddMappingContext(InteriorMapping, 0);
            if (ArchVizWidgetInstance) {
                InteriorWidgetInstance->AddToViewport();
                SpawnCustomBuildingActor();
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Interior Mode"));
            }
            
        }
        else if (InSelectedItem == "Material Selection") {
            Subsystem->AddMappingContext(MaterialMapping, 0);
            if (MaterialWidgetInstance) {
                MaterialWidgetInstance->AddToViewport();
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Material Mode"));
            }
        }
        else {
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Invalid Mode"));
        }
       
    }
}
void AArchVizExplorerController::ClearViewportWidgets() {
    if (RoadWidgetInstance&& MaterialWidgetInstance && BuildingWidgetInstance && InteriorWidgetInstance) {
        RoadWidgetInstance->RemoveFromViewport();
        MaterialWidgetInstance->RemoveFromViewport();
        BuildingWidgetInstance->RemoveFromViewport();
        InteriorWidgetInstance->RemoveFromViewport();
    }
}

void AArchVizExplorerController::OnDestroyRoad() {
    if (ProceduralRoadActor) {
        ProceduralRoadActor->Destroy();
        ProceduralRoadActor = nullptr;
    }
}
void AArchVizExplorerController::OnDestroyInterior()
{
    if (CustomInteriorInstance) {
        CustomInteriorInstance->Destroy();
        CustomInteriorInstance = nullptr;    
    }
}
void AArchVizExplorerController::OnRotateInterior()
{
    if (CustomInteriorInstance) {
        FRotator Rotation = CustomInteriorInstance->GetActorRotation();
        CustomInteriorInstance->SetActorRotation(Rotation + FRotator{ 0,90,0 });  
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Rotated"));
    }
}
void AArchVizExplorerController::OnDestroyCustomBuidling()
{
    if (CustomBuildingInstance) {
        CustomBuildingInstance->Destroy();
        CustomBuildingInstance = nullptr;
    }
}
void AArchVizExplorerController::OnLeftMouseClick()
{
    FHitResult HitResult;
    FVector WorldLocation, WorldDirection;
    DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
    FVector HitLocation = HitResult.ImpactPoint;
    AProcRoadActor* HitRoadActor = Cast<AProcRoadActor>(HitResult.GetActor());
    if (HitRoadActor) {
        ProceduralRoadActor = HitRoadActor;
        double Width = ProceduralRoadActor->GetActorRelativeScale3D().Y;
        FVector RoadLocation = ProceduralRoadActor->GetActorLocation();
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
            ProceduralRoadActor = GetWorld()->SpawnActor<AProcRoadActor>(ProceduralRoadActor->StaticClass(),MiddlePoint,Rotation,SpawnParams);
            //float  Length, Width, Height;
            if (ProceduralRoadActor) {
                ProceduralRoadActor->GenerateRoadMesh(Length, 100, 3);
            }
            if (RoadWidgetInstance) {
                FVector RoadLocation = ProceduralRoadActor->GetActorLocation();
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
    
    AProcRoadActor* HitRoadActor = Cast<AProcRoadActor>(HitResult.GetActor());
    if (HitRoadActor) {
        MaterialWidgetInstance->MaterialScrollBox->SetVisibility(ESlateVisibility::Visible);
        ProceduralRoadActor = HitRoadActor;
    }
    else {
        MaterialWidgetInstance->MaterialScrollBox->SetVisibility(ESlateVisibility::Hidden);
    }
    ACustomBuildingActor* HitBuilidngActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
    if (HitBuilidngActor) {
        MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Visible);

        CustomBuildingInstance = HitBuilidngActor;

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("Actor Selected"));
    }
    else {
        MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Hidden);
    }

}
void AArchVizExplorerController::ApplyBuildingMaterial(const FBuildingMaterialData& BuildingMaterialData) {
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Applying"));
    if (CustomBuildingInstance) {
        CustomBuildingInstance->SetMaterial(BuildingMaterialData.BuildingMaterialReference);
        CustomBuildingInstance = nullptr;
    }
}
void AArchVizExplorerController::B_LeftMouseClick() {
    //bMove = false;
    BuildingWidgetInstance->DestroyComponent->SetVisibility(ESlateVisibility::Hidden);
    FHitResult HitResult;
    FVector WorldLocation, WorldDirection;
    DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
    FVector HitLocation = HitResult.ImpactPoint;
    if (HitResult.IsValidBlockingHit()&& CustomBuildingInstance) {
        if (CustomBuildingInstance->GetActorType() == "Roof") {

            HitResult.Location.Z = 420.0f;
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Roof"));
        }
        else {
            HitResult.Location.Z = 0.0f;
        }
            CustomBuildingInstance->SetActorLocation(HitResult.Location);
            SnapActor(20.0f);
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Actor Placed"));
            CustomBuildingInstance = nullptr;
            //SpawnCustomBuildingActor();
    }
}
void AArchVizExplorerController::B_RightMouseClick() {
    FHitResult HitResult;
    FVector WorldLocation, WorldDirection;
    DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);

    // Check if the hit actor is a valid CustomBuildingActor and different from the current CustomBuildingInstance
    ACustomBuildingActor* HitActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
    if (HitActor && HitActor != CustomBuildingInstance) {
        BuildingWidgetInstance->DestroyComponent->SetVisibility(ESlateVisibility::Visible);
        CustomBuildingInstance = HitActor;
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Found CustomBuildingActor"));
    }
    else {
        BuildingWidgetInstance->DestroyComponent->SetVisibility(ESlateVisibility::Hidden);
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("No CustomBuildingActor found"));
    }
}
void AArchVizExplorerController::SpawnCustomBuildingActor() {

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        CustomBuildingInstance = GetWorld()->SpawnActor<ACustomBuildingActor>(CustomBuildingClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("New actor spawned"));

}
void AArchVizExplorerController::RotateCustomBuilding() {
   
    FRotator NewRotation = FRotator(0, 90,0);
    CustomBuildingInstance->SetActorRelativeRotation(CustomBuildingInstance->GetActorRotation()+ NewRotation);
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Rotation Changed"));
}


void AArchVizExplorerController::SetNumberOfSegments(int32 Number) {
    if (CustomBuildingInstance) {

        NumberOfSegments = Number;
        CustomBuildingInstance->SetMeshScale(NumberOfSegments);
    }
    
    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("%d"),NumberOfSegments);
}

void AArchVizExplorerController::SnapActor(float SnapValue)
{

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
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Unplaced actor destroyed"));
    }
}

//void AArchVizExplorerController::PlaceDoor(const FVector& HitLocation)
//{
//    FHitResult OverlapResult;
//    FVector Start = HitLocation;
//    FVector End = Start + FVector(0, 0, 100); // Check 100 units up for overlapping walls
//
//    // Perform a sweep to find overlapping walls
//    FCollisionShape CollisionShape = FCollisionShape::MakeBox(FVector(50, 50, 100)); // Size of the door
//    if (GetWorld()->SweepSingleByChannel(OverlapResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, CollisionShape)) {
//        ACustomBuildingActor* OverlappingWall = Cast<ACustomBuildingActor>(OverlapResult.GetActor());
//        if (OverlappingWall && OverlappingWall->GetActorType() == "Wall") {
//            // Replace the wall section with a new wall mesh that has a door cut-out
//            FVector WallLocation = OverlappingWall->GetActorLocation();
//            FRotator WallRotation = OverlappingWall->GetActorRotation();
//            OverlappingWall->Destroy();
//
//            FActorSpawnParameters SpawnParams;
//            SpawnParams.Owner = this;
//            ACustomBuildingActor* NewWallWithDoor = GetWorld()->SpawnActor<ACustomBuildingActor>(CustomBuildingClass, WallLocation, WallRotation, SpawnParams);
//
//            // Place the door at the location
//            CustomBuildingInstance->SetActorLocation(HitLocation);
//            SnapActor(20.0f);
//
//            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Door placed and overlapping wall replaced"));
//        }
//    }
//    else {
//        // Place the door normally if no wall overlaps
//        CustomBuildingInstance->SetActorLocation(HitLocation);
//        SnapActor(20.0f);
//
//        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Door placed without overlapping wall"));
//    }
//}

void AArchVizExplorerController::I_LeftMouseClick()
{

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("FRom Interior left cllicked"));
    FHitResult HitResult;
    FVector WorldLocation, WorldDirection;
    DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
    FVector HitLocation = HitResult.ImpactPoint;
    ACustomBuildingActor* HitBuilidngActor = Cast<ACustomBuildingActor>(HitResult.GetActor());
    if (HitBuilidngActor) {
        CustomBuildingInstance = HitBuilidngActor;
        if (CustomBuildingInstance->GetActorType().Equals(FString{ "Floor" })) {
            InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Visible);

            InteriorHitLocation = HitLocation;
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT(" Floor Location Selected"));
        }
        else {

        InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else {
        //DestroyUnplacedActor();
        InteriorWidgetInstance->InteriorScrollBox->SetVisibility(ESlateVisibility::Hidden);
        
    }
    if (HitBuilidngActor) {
        CustomBuildingInstance = HitBuilidngActor;
        if (CustomBuildingInstance->GetActorType().Equals(FString{ "Wall" })) {
            InteriorWidgetInstance->WallInteriorScrollBox->SetVisibility(ESlateVisibility::Visible);

            InteriorHitLocation = HitLocation;
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT(" Wall Location Selected"));
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

            InteriorHitLocation = HitLocation;
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT(" Wall Location Selected"));
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
        InteriorWidgetInstance->DestroyInterior->SetVisibility(ESlateVisibility::Visible);
        InteriorWidgetInstance->RotateInterior->SetVisibility(ESlateVisibility::Visible);
        InteriorWidgetInstance->InteriorLocationX->SetValue(CustomInteriorInstance->GetActorLocation().X);
        InteriorWidgetInstance->InteriorLocationY->SetValue(CustomInteriorInstance->GetActorLocation().Y);
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("Interior Selected"));

    }
    else {
        InteriorWidgetInstance->DestroyInterior->SetVisibility(ESlateVisibility::Hidden);
        InteriorWidgetInstance->RotateInterior->SetVisibility(ESlateVisibility::Hidden);

    }

}
FVector AArchVizExplorerController::GetActorExtentFromMesh(UStaticMesh* Mesh) const {
    if (Mesh) {
        FVector Origin, BoxExtent;
        Mesh->GetBounds().GetBox().GetCenterAndExtents(Origin, BoxExtent);
        return BoxExtent;
    }
    return FVector(50.0f, 50.0f, 50.0f); // Default extent if no mesh is provided
}

bool AArchVizExplorerController::CheckCollisionAtLocation(const FVector& Location, const FVector& ActorExtent, const TArray<FString>& IgnoredActorTypes, ACustomBuildingActor* ActorToIgnore)
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
                ACustomBuildingActor* CustomBuildingActor = Cast<ACustomBuildingActor>(OverlappingActor);
                if (CustomBuildingActor) {
                    FString ActorType = CustomBuildingActor->GetActorType();
                    if (IgnoredActorTypes.Contains(ActorType)) {
                        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Collision detected with %s."), *ActorType));
                        return true;
                    }
                }
                else {
                    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Overlapping actor is not of type ACustomBuildingActor."));
                }
            }
        }
    }
    return false;
}
void AArchVizExplorerController::PlaceInterior(const FInteriorData& InteriorData) {
    FVector ActorExtent = GetActorExtentFromMesh(InteriorData.InteriorTypeReference);
    TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door" };

    if (CheckCollisionAtLocation(InteriorHitLocation, ActorExtent, IgnoredActorTypes)) {
        return;
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("No collision detected, placing interior."));

    if (CustomInteriorClass) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        CustomInteriorInstance = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorHitLocation, FRotator::ZeroRotator, SpawnParams);
        if (CustomInteriorInstance) {
            CustomInteriorInstance->SetStaticMesh(InteriorData.InteriorTypeReference);
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Interior actor placed successfully."));
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

    if (CheckCollisionAtLocation(InteriorHitLocation, ActorExtent, IgnoredActorTypes, SelectedWallActor)) {
        return;
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("No collision detected, placing interior."));

    if (CustomInteriorClass) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        CustomInteriorInstance = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorHitLocation, FRotator::ZeroRotator, SpawnParams);
        if (CustomInteriorInstance) {
            CustomInteriorInstance->SetStaticMesh(WallInteriorData.WallInteriorTypeReference);
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Interior actor placed successfully."));
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
    TArray<FString> IgnoredActorTypes = { "Floor", "Wall", "Door" };
    ACustomBuildingActor* SelectedCeilingActor = CustomBuildingInstance;

    if (CheckCollisionAtLocation(InteriorHitLocation, ActorExtent, IgnoredActorTypes, SelectedCeilingActor)) {
        return;
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("No collision detected, placing interior."));

    if (CustomInteriorClass) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        CustomInteriorInstance = GetWorld()->SpawnActor<ACustomInteriorActor>(CustomInteriorClass, InteriorHitLocation, FRotator::ZeroRotator, SpawnParams);
        if (CustomInteriorInstance) {
            CustomInteriorInstance->SetStaticMesh(CeilingInteriorData.CeilingInteriorTypeReference);
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Interior actor placed successfully."));
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
        TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door" };

        ACustomBuildingActor* CustomBuildingActor = CustomBuildingInstance;
        if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("New location is not placeable. Collision detected."));
            CustomInteriorInstance->SetActorLocation(OldLocation);
            return;
        }

        CustomInteriorInstance->SetActorLocation(NewLocation);
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("x"));
    }
}

void AArchVizExplorerController::OnInteriorLocationYChanged(float InValue) {
    if (CustomInteriorInstance) {
        FVector OldLocation = CustomInteriorInstance->GetActorLocation();
        FVector NewLocation = OldLocation;
        NewLocation.Y = InValue;
        UStaticMesh* Mesh = CustomInteriorInstance->GetStaticMesh();
        FVector ActorExtent = GetActorExtentFromMesh(Mesh);
        TArray<FString> IgnoredActorTypes = { "Wall", "Roof", "Door" };

        ACustomBuildingActor* CustomBuildingActor = CustomBuildingInstance;
        if (CheckCollisionAtLocation(NewLocation, ActorExtent, IgnoredActorTypes, CustomBuildingActor)) {
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("New location is not placeable. Collision detected."));
            CustomInteriorInstance->SetActorLocation(OldLocation);
            return;
        }

        CustomInteriorInstance->SetActorLocation(NewLocation);
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("y"));
    }
}
