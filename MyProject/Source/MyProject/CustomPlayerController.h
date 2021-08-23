// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <algorithm>

#include "GameFramework/PlayerController.h"
#include "Chessboard.h"
#include "ChessPieces.h"
#include "ParentActor.h"

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

	/************** PHYSICAL INTERACT FUNCTION **************/

/* 
	SelectPiece() function contains:
	1. Convert from the location -> index
	2. Check to see if the piece is in our side (controllable)
	3. Get the piece Value (and see what Type of Piece it is)
	4. Highlight the selectable tiles
*/

	UFUNCTION()
	void HighlightSelectedTileByLocation(const FVector& hitLocation);

	UFUNCTION()
	void HighlightSelectedTileByIndex(const int16& indexToHighlight);

	UFUNCTION()
	void HighlightMovableIndices(const int16& startMovableIndex);

	UFUNCTION()
	void UnhighlightTileByIndex(const int16& index, const bool& bMarkRenderStateDirty);

	UFUNCTION()
	FVector2D LocationToXY(const FVector& hitLocation);

	UFUNCTION()
	int16 LocationToIndex(const FVector& hitLocation);

	UFUNCTION()
	int16 XYToIndex(const int16& XValue, const int16& YValue);

	// just consider change int16->int8 if possible on all the codes
	UFUNCTION()
	void checkMateToEnemyTeam(const int16& pieceIndex);

	UFUNCTION()
	void checkMateToThisTeam();

	UFUNCTION()
	FVector Index196ToLocation(const int16& index);

	UFUNCTION()
	bool isIndexValid(const int16& index);

	// Validation of Click1
	UFUNCTION()
	bool isClick1P1IndexValid(const int16& hitIndexOn196);

	UFUNCTION()
	bool isClick1P2IndexValid(const int16& hitIndexOn196);

	UFUNCTION()
	bool isClick1P3IndexValid(const int16& hitIndexOn196);

	UFUNCTION()
	bool isClick1P4IndexValid(const int16& hitIndexOn196);

	// Validation of Click2
	UFUNCTION()
	bool isClick2MoveIndexValid(const int16& hitIndexOn196);

	// checking what kind of piece it's
	UFUNCTION()
	bool isPawn(const int16& pieceValue);

	UFUNCTION()
	bool isRook(const int16& pieceValue);

	UFUNCTION()
	bool isKnight(const int16& pieceValue);

	UFUNCTION()
	bool isBishop(const int16& pieceValue);

	UFUNCTION()
	bool isQueen(const int16& pieceValue);

	UFUNCTION()
	bool isKing(const int16& pieceValue);

	UFUNCTION()
	void makeMove(const int16& index1, int16 index2);

	UFUNCTION()
	void moveToGraveyard(const int16& index);

	UFUNCTION()
	int16 LeftThresholdOfIndex(const int16& index);

	UFUNCTION()
	int16 RightThresholdOfIndex(const int16& index);

	//UFUNCTION()
	//void SwitchPlayer();

	//UFUNCTION(BlueprintPure)
	//AChessPlayer* GetCurrentPlayer();

	// TODO: remember to exchange/swap the ChessBoard_Values(mechanics) when move a Piece
	//UFUNCTION(BlueprintCallable)
	//void MakeMove();

	//UFUNCTION(BlueprintCallable)
	//void Cancel();

	/************** MECHANIC FUNCTION **************/

	//UFUNCTION()
	//void GetMovableIndices(int16 pieceValue, FVector clickLocation, TArray<int16> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesPawn(int16 pieceValue, FVector clickLocation, TArray<int16> MovableIndices); 

	UFUNCTION()
	void GetMovableIndicesRook(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign); 

	UFUNCTION()
	void GetMovableIndicesKnight(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign); 

	UFUNCTION()
	void GetMovableIndicesBishop(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign); 

	UFUNCTION()
	void GetMovableIndicesQueen(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign); 

	UFUNCTION()
	void GetMovableIndicesKing(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign); 

	UFUNCTION()
	void GetMovableIndicesPawn(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign); 
	//UFUNCTION()
	//void GetMovableIndicesKnight(int16 pieceValue, FVector clickLocation, TArray<int16> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesBishop(int16 pieceValue, FVector clickLocation, TArray<int16> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesKing(int16 pieceValue, FVector clickLocation, TArray<int16> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesQueen(int16 pieceValue, FVector clickLocation, TArray<int16> MovableIndices); 

	UFUNCTION()
	void spawnBottomActors();
	UFUNCTION()
	void spawnLeftActors();
	UFUNCTION()
	void spawnTopActors();
	UFUNCTION()
	void spawnRightActors();

	/************** MECHANIC ARRAYS **************/
	// We have to think of the real mechanics Array is SEPARATE from the Physical Board and Pieces
	// So on each move we have to alter/change values of BOTH
	TArray<int16, TFixedAllocator<196>> IndexInPieceVectorFromBoardIndex;		// this TInlineAllocator make sure to use the Stack(fixed) Memory for the first 196 items of TArray
	TArray<uint8, TFixedAllocator<160>> MovableIndices;
	TArray<AParentActor*, TFixedAllocator<64>> pieces;

	TArray<int16, TFixedAllocator<196>> EatableHashKeys;


	/************** PHYSICAL PROPERTIES **************/

	// Chessboard
	UPROPERTY()
	AChessBoard* Board;

	// ChessPieces
	UPROPERTY()
	AChessPieces* Pieces;

protected:

	/************** PLAYER INPUT SETUP **************/

	// ~Overrides: APlayerController
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
private:

	int16 ActiveIndex;
	int16 clickCount;
	int16 hitIndexOn196_1;

	bool bEnPassant;

	int8 playerTurn;

	bool bHash;

	uint8 playerBeingCheckedMate;

	bool bP1Checked;
	bool bP2Checked;
	bool bP3Checked;
	bool bP4Checked;
};
