// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class MYPROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	// Sets default values for this pawn's properties
	APlayerPawn();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Call every frame
	virtual void Tick(float DeltaTime) override;

public:	

	/**** CAMERA VARIABLES ****/

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void MoveHigher(float AxisValue);

	void SmoothZoomIn();

	void SmoothZoomOut();

	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);

    UFUNCTION()
    void OnLeftMouseClick(FVector hitLocation);

    UFUNCTION(Server,Reliable)
    void Server_OnLeftMouseClick(FVector hitLocation);

    UFUNCTION(NetMulticast,Reliable)
    void Multicast_OnLeftMouseClick(FVector hitLocation);
	/**** END OF AMERA VARIABLES ****/

	// Gate Logic
	void GateOpen();

	void GateClose();

	/**** END OF CAMERA SPECS ****/

    UPROPERTY()
    ASpawnManager* chessManager;

protected:

	/**** CAMERA COMPONENTS ****/

	USpringArmComponent* SpringArmComp;

	UCameraComponent* CameraComp;

	/**** CAMERA SPECS ****/

	UStaticMeshComponent* StaticMeshComp;	// This static mesh comp so that we can see the camera physically and debug it

	FVector MovementInput;

	FVector2D CameraInput;	

	float MinTargetLength;	// Sets the Minimum Target Arm Length 

	float MaxTargetLength;	// Sets the Maximum Target Arm Length 

	float ZoomUnits;	// Determines the number of units are added or subtracted from the target arm length 

	float ZoomSmoothness;	// How smooth the zoom will be (Higher less smooth, lower more smooth 

private:

	float DesiredArmLength;

	bool bIsOpen;	// Gate Logic Variable 

};
