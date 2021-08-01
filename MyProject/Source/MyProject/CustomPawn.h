// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomPLayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include <Camera/CameraComponent.h>

#include "CustomPawn.generated.h"

UCLASS()
class MYPROJECT_API ACustomPawn : public APawn
{
	GENERATED_BODY()

public:

	// Sets default values for this pawn's properties
	ACustomPawn();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	

	UPROPERTY()
	ACustomPlayerController* PlayerController;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);

	void SmoothZoomIn();

	void SmoothZoomOut();

	void GateOpen();

	void GateClose();

	// Call every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY()
	USpringArmComponent* SpringArmComp;

	UPROPERTY()
	UCameraComponent* CameraComp;

	// This static mesh comp so that we can see the camera physically and debug it
	UPROPERTY(VisibleAnywhere, Category="CameraSpecs")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, Category="CameraSpecs")
	FVector2D MovementInput;

	UPROPERTY(VisibleAnywhere, Category="CameraSpecs")
	FVector2D CameraInput;	

	UPROPERTY(VisibleAnywhere, Category="CameraSpecs")
	bool bZoomOut;

	/* Sets the Minimum Target Arm Length */
	UPROPERTY(VisibleAnywhere, Category = "Camera Zoom")
	float MinTargetLength;

	/* Sets the Maximum Target Arm Length*/
	UPROPERTY(VisibleAnywhere, Category = "Camera Zoom")
	float MaxTargetLength;

	/* Determines the number of units are added or subtracted from the target arm length*/
	UPROPERTY(VisibleAnywhere, Category = "Camera Zoom")
	float ZoomUnits;

	/*How smooth the zoom will be (Higher less smooth, lower more smooth*/
	UPROPERTY(VisibleAnywhere, Category = "Camera Zoom")
	float ZoomSmoothness;

	bool bIsOpen;

private:

	float DesiredArmLength;

};
