// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController2.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomPlayerController2 : public APlayerController
{
	GENERATED_BODY()

public:

	ACustomPlayerController2();

	/************** EVENT BINDINGS ON INPUT **************/
	UFUNCTION()
	void OnLeftMouseClick();

	UFUNCTION()
	void OnShiftLeftMouseClick();

	// ~Overrides: APlayerController
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
};
