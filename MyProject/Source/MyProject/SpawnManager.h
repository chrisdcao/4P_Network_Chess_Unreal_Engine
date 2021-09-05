// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chessboard.h"
#include "ChessPieces.h"
#include "ParentActor.h"
#include "SpawnManager.generated.h"

UCLASS()
class MYPROJECT_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	/************** PHYSICAL INTERACT FUNCTION **************/

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

	/************** MECHANIC ARRAYS **************/
    //UPROPERTY(Replicated)
	TArray<int16> BoardOfPieceIndexInVector;

    UPROPERTY()
	TArray<uint8> MovableIndices;

    //UPROPERTY(Replicated)
	TArray<AParentActor*> PiecesVector;

    UPROPERTY()
	TArray<int16> EatableHashKeys;

	/************** PHYSICAL PROPERTIES **************/

	// Chessboard
	//UPROPERTY(ReplicatedUsing=OnRep_Board)
	AChessBoard* Board;

    UFUNCTION()
    void OnLeftMouseClick(FVector hitLocation);

	// this might have to be in CustomGameMode for the game mode to monitor
	int8 playerTurn;

private:

	int16 clickCount; // TODO: this should be migrated to Pawn instead of SpawnManager?
	int16 hitIndexOnBoard_1;
	int16 ActiveIndex;

    UPROPERTY(VisibleAnywhere, Category="SpecialMoves|EnPassant")
	bool bEnPassant;

	// TODO: this might be to GameState? or PlayerState? In general, this is for ending the game rather than individual scores
    // probably use Replicated Using = ? because we don't want update this over the net per frequency
    UPROPERTY(VisibleAnywhere, Category="CheckingStatus|Player1")
	bool bP1Checked;
    UPROPERTY(VisibleAnywhere, Category="CheckingStatus|Player2")
	bool bP2Checked;
    UPROPERTY(VisibleAnywhere, Category="CheckingStatus|Player3")
	bool bP3Checked;
    UPROPERTY(VisibleAnywhere, Category="CheckingStatus|Player4")
	bool bP4Checked;

	// TODO: this might be to each Pawn. We might have to cast each pawn on each login to each separate pawn (rather than using 1 united default pawn like in single player game)
	// TODO: or as keeping track individually like this it might be in PlayerState
    // so this doesn't need to be updated to the server?
    UPROPERTY(VisibleAnywhere, Category="SpecialMoves|Castling")
	bool bP1Castling;
    UPROPERTY(VisibleAnywhere, Category="SpecialMoves|Castling")
	bool bP2Castling;
    UPROPERTY(VisibleAnywhere, Category="SpecialMoves|Castling")
	bool bP3Castling;
    UPROPERTY(VisibleAnywhere, Category="SpecialMoves|Castling")
	bool bP4Castling;

};
