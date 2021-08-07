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
	void HighlightSelectedTileByIndex(const int32& indexToHighlight);

	UFUNCTION()
	void HighlightMovableIndices(const int32& startMovableIndex);

	UFUNCTION()
	void UnhighlightTileByIndex(const int32& index, const bool& bMarkRenderStateDirty);

	UFUNCTION()
	FVector2D LocationToXY(const FVector& hitLocation);

	UFUNCTION()
	int32 LocationToIndex(const FVector& hitLocation);

	UFUNCTION()
	int32 XYToIndex(const int32& XValue, const int32& YValue);

	UFUNCTION()
	void exchange(const int32& index1, const int32& index2, TArray<int32> array);

	// just consider change int32->int8 if possible on all the codes
	UFUNCTION()
	bool checkMate(const int32& playerIndex);

	UFUNCTION()
	FVector IndexToLocation(const int32& index);

	UFUNCTION()
	bool isIndexValid(const int32& index);

	// Validation of Click1
	UFUNCTION()
	bool isClick1P1IndexValid(const int32& hitIndex);

	UFUNCTION()
	bool isClick1P2IndexValid(const int32& hitIndex);

	UFUNCTION()
	bool isClick1P3IndexValid(const int32& hitIndex);

	UFUNCTION()
	bool isClick1P4IndexValid(const int32& hitIndex);

	// Validation of Click2
	UFUNCTION()
	bool isClick2MoveIndexValid(const int32& hitIndex);

	// checking what kind of piece it's
	UFUNCTION()
	bool isPawn(const int32& pieceValue);

	UFUNCTION()
	bool isRook(const int32& pieceValue);

	UFUNCTION()
	bool isKnight(const int32& pieceValue);

	UFUNCTION()
	bool isBishop(const int32& pieceValue);

	UFUNCTION()
	bool isQueen(const int32& pieceValue);

	UFUNCTION()
	bool isKing(const int32& pieceValue);

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
	//void GetMovableIndices(int32 pieceValue, FVector clickLocation, TArray<int32> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesPawn(int32 pieceValue, FVector clickLocation, TArray<int32> MovableIndices); 

	UFUNCTION()
	void GetMovableIndicesRook(int32& startStorageIndex, const FVector& clickLocation); 

	UFUNCTION()
	void GetMovableIndicesKnight(int32& startStorageIndex, const FVector& clickLocation); 

	UFUNCTION()
	void GetMovableIndicesBishop(int32& startStorageIndex, const FVector& clickLocation); 

	UFUNCTION()
	void GetMovableIndicesQueen(int32& startStorageIndex, const FVector& clickLocation); 

	UFUNCTION()
	void GetMovableIndicesKing(int32& startStorageIndex, const FVector& clickLocation); 

	UFUNCTION()
	void GetMovableIndicesPawn(int32& startStorageIndex, const FVector& clickLocation); 
	//UFUNCTION()
	//void GetMovableIndicesKnight(int32 pieceValue, FVector clickLocation, TArray<int32> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesBishop(int32 pieceValue, FVector clickLocation, TArray<int32> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesKing(int32 pieceValue, FVector clickLocation, TArray<int32> MovableIndices); 
	//UFUNCTION()
	//void GetMovableIndicesQueen(int32 pieceValue, FVector clickLocation, TArray<int32> MovableIndices); 

	/************** MECHANIC ARRAYS **************/
	// We have to think of the real mechanics Array is SEPARATE from the Physical Board and Pieces
	// So on each move we have to alter/change values of BOTH
	TArray<int32> ChessBoardValues;
	TArray<int32> ISMIndexMapOn196Board;
	// HOLDS the indices of Movable Tiles
	// TODO: Consider adding another array holding Our Team and Enemy Team MovableIndices differently for later CheckMate function
	TArray<int32> MovableIndices;
	// HOLDS the indices of Movable Tiles for KING ONLY
	TArray<int32> KingP1MovableIndices;
	TArray<int32> KingP2MovableIndices;
	TArray<int32> KingP3MovableIndices;
	TArray<int32> KingP4MovableIndices;


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

	int32 Click1Value;
	int32 Click2Value;
	int32 ActiveIndex;
	int32 clickCount;
	int32 hitIndex1;
	bool bEnPassant;

};

