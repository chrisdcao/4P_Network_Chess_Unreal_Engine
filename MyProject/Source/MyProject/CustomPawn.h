// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomPLayerController.h"
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

public:	

	UPROPERTY()
	ACustomPlayerController* PlayerController;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY()
	USpringArmComponent* SpringArmComp;

	UPROPERTY()
	UCameraComponent* CameraComp;

	// This static mesh comp so that we can see the camera physically and debug it
	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMeshComponent* StaticMeshComp;

};
