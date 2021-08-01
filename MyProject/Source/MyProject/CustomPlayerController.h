// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Chessboard.h"
#include "ChessPieces.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACustomPlayerController();


	/************** EVENTS ON INPUT BINDINGS **************/
	UFUNCTION()
	void OnLeftMouseClick();

	UFUNCTION()
	void OnShiftLeftMouseClick();

	UFUNCTION()
	bool isClickValid(const FVector& hitLocation);


	/************** PHYSICAL INTERACT FUNCTION **************/

/* 
	SelectPiece() function contains:
	1. Convert from the location -> index
	2. Check to see if the piece is in our side (controllable)
	3. Get the piece Value (and see what Type of Piece it is)
	4. Highlight the selectable tiles
*/

	UFUNCTION()
	void SelectPiece();		

	UFUNCTION()
	void HighlightTiles(const FVector& hitLocation);

	UFUNCTION()
	int LocationToIndex(const FVector& hitLocation);

	//UFUNCTION()
	//void SwitchPlayer();

	//UFUNCTION(BlueprintPure)
	//AChessPlayer* GetCurrentPlayer();

	//UFUNCTION(BlueprintCallable)
	//void MakeMove();

	//UFUNCTION(BlueprintCallable)
	//void Cancel();

	/************** MECHANIC FUNCTION **************/

	/************** MECHANIC ARRAYS **************/
	// We have to think of the real mechanics Array is SEPARATE from the Physical Board and Pieces
	// So on each move we have to alter/change values of BOTH
	TArray<int32> ChessBoard_Values;


	/************** PHYSICAL ARRAYS **************/

	// Chessboard
	AChessBoard* Board;

	// ChessPieces
	AChessPieces* Pieces;

protected:

	/************** PLAYER INPUT SETUP **************/
	// TODO: Setup Panning and Moving Cameras
	// You may also want to think of adding Input for Panning/Rotating Camera using Keyboards

	// ~Overrides: APlayerController
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
private:

	int32 Click1Value;
	int32 Click2Value;

};

