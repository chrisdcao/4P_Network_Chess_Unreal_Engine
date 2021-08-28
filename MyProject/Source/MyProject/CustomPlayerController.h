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

	/************** EVENT BINDINGS ON INPUT **************/
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
	void checkMateToWholeTeamOfPlayerNumber(const int16& playerNumber);

	UFUNCTION()
	bool checkMateFromPlayerXToPlayerY(const int16& playerX, const int16& playerY);

	UFUNCTION()
	FVector BoardIndexToLocation(const int16& index);

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

	/************** MECHANIC FUNCTION **************/

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

	UFUNCTION()
	void spawnBottomActors();
	UFUNCTION()
	void spawnLeftActors();
	UFUNCTION()
	void spawnTopActors();
	UFUNCTION()
	void spawnRightActors();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/************** MECHANIC ARRAYS **************/
	TArray<int16, TFixedAllocator<196>> BoardOfPieceIndexInVector;		// this TInlineAllocator make sure to use the Stack(fixed) Memory for the first 196 items of TArray
	TArray<uint8, TFixedAllocator<160>> MovableIndices;
	TArray<AParentActor*, TFixedAllocator<64>> PiecesVector;
	TArray<int16, TFixedAllocator<196>> EatableHashKeys;

	/************** PHYSICAL PROPERTIES **************/

	// Chessboard
	UPROPERTY(Replicated)
	AChessBoard* Board;

	// ChessPieces
	UPROPERTY(Replicated)
	AChessPieces* Pieces;

protected:

	/************** PLAYER INPUT SETUP **************/

	// ~Overrides: APlayerController
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	
private:

	int16 clickCount;
	int16 hitIndexOnBoard_1;
	int16 ActiveIndex;
	int8 playerTurn;

	bool bEnPassant;

	bool bP1Checked;
	bool bP2Checked;
	bool bP3Checked;
	bool bP4Checked;

	bool bP1Castling;
	bool bP2Castling;
	bool bP3Castling;
	bool bP4Castling;
};
