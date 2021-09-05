// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController2.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"

ACustomPlayerController2::ACustomPlayerController2()
{

    bReplicates = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;

}

void ACustomPlayerController2::SetupInputComponent()
{
	// Always call this.
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ACustomPlayerController2::OnLeftMouseClick);
	UE_LOG(LogTemp, Warning, TEXT("Non abstract"));
}

void ACustomPlayerController2::OnLeftMouseClick()
{
    FHitResult TraceResult(ForceInit);
    GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, OUT TraceResult);
    FVector hitLocation = TraceResult.Location;
    controlledPawn->OnLeftMouseClick(hitLocation);
}

void ACustomPlayerController2::OnShiftLeftMouseClick()
{

}

void ACustomPlayerController2::BeginPlay()
{
	Super::BeginPlay();
    controlledPawn = Cast<APlayerPawn>(GetPawn());
}
