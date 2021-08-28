// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController2.h"

ACustomPlayerController2::ACustomPlayerController2()
{

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;

}

void ACustomPlayerController2::OnLeftMouseClick()
{

}

void ACustomPlayerController2::OnShiftLeftMouseClick()
{

}

void ACustomPlayerController2::SetupInputComponent()
{
	// Always call this.
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("Non abstract"));
}

void ACustomPlayerController2::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Non abstract"));
}
