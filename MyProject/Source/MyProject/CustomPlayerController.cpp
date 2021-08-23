// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "ChessBoard.h"
#include "ChessPieces.h"
#include "RookActor.h"
#include "KnightActor.h"
#include "BishopActor.h"
#include "KingActor.h"
#include "QueenActor.h"
#include "PawnActor.h"
#include "ParentActor.h"
#include <algorithm>

#define OUT

// Currently non-compilable as the functions are not completed yet
// TODO: CHANGE SET MESH TO THIS? CHANGE TO ANYTHING THAT SUPPORT PLAYER PICKING SKIN
ACustomPlayerController::ACustomPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;

	playerTurn = 1;
	clickCount = 0;
	hitIndexOn196_1 = -1;
	// init all the mechanics arrays and variables here
	// remember to exchange the value of the mechanic array whenever you move
	ActiveIndex = 0;
	bEnPassant = false;
	MovableIndices.Init(0, 160);
	MovableHashKeys.Init(-1, 196);

	bHash = false;
	IndexInPieceVectorFromBoardIndex = {
		-2, -2, -2, 8, 10, 12, 14, 15, 13, 11, 9, -2, -2, -2,
		-2, -2, -2, 0, 1, 2, 3, 4, 5, 6, 7, -2, -2, -2,
		-2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2,
		40, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 48, 56,
		42, 33, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 49, 58,
		44, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 50, 60,
		46, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 51, 63,
		47, 36, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 52, 62,
		45, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 53, 61,
		43, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 54, 59,
		41, 39, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 55, 57,
		-2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2,
		-2, -2, -2, 16, 17, 18, 19, 20, 21, 22, 23, -2, -2, -2,
		-2, -2, -2, 24, 26, 28, 31, 30, 29, 27, 25, -2, -2, -2
	};

}

void ACustomPlayerController::SetupInputComponent()
{
	// Always call this.
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ACustomPlayerController::OnLeftMouseClick);
	InputComponent->BindAction("ShiftLeftMouseClick", IE_Pressed, this, &ACustomPlayerController::OnShiftLeftMouseClick);
}

// this is for Planning Attack between teammates
void ACustomPlayerController::OnShiftLeftMouseClick()
{

}


// TODO: THIS IS HAVING SOME TROUBLE RESETTING CLICKS
// TODO: begin to divide on different player turn and check if currentIndex Piece belongs to that person?
// TODO: hitValue1 is being wrong (must take pieces[]->value)
void ACustomPlayerController::OnLeftMouseClick()
{
	FHitResult TraceResult(ForceInit);
	// might have to change the collision Channel to Visible, because Our objects are in 1 mesh (thus cannot detect the different type of ECC for each object)
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, OUT TraceResult);
	FVector hitLocation = TraceResult.Location;

	if (playerTurn == 1)
	{   // the events happening in Click1
		if (bP1Checked && !bP3Checked)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player1 is currently being checkedMate!"));
		}
		else if (bP3Checked && !bP1Checked)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player3 is currently being checkedMate!"));
		}
		else if (bP1Checked && bP3Checked)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player1 and 3 are currently being checkedMate!"));
		}

		if (clickCount == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Left mouse click 1 is triggered!"));
			UE_LOG(LogTemp, Warning, TEXT("current clickCount is %d"), clickCount);

			hitIndexOn196_1 = LocationToIndex(hitLocation);
			// TODO: make this 4 players 
			if (!isClick1P1IndexValid(hitIndexOn196_1)) return;

			clickCount = 1;

			const int16 hitValue1 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
			UE_LOG(LogTemp, Warning, TEXT("current clickvalue is %d"), hitValue1);

			int16 startStorageIndex = 0;
			HighlightSelectedTileByIndex(hitIndexOn196_1);

			UE_LOG(LogTemp, Warning, TEXT("Hit Value is %d!"), IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]);
			UE_LOG(LogTemp, Warning, TEXT("Hit Index is %d!"), hitIndexOn196_1);
			if (isPawn(hitValue1))
			{
				UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
				GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		// TODO: MIGHT HAVE TO CHANGE THE CONDITION CHECKING OF CLICK1 AND CLICK2 TO EXCLUDE 0 AND MAKE IT A SEPARATE CHECK
		///TODO: CHECK IF INDEX IS THE SAME, if the same then we repeat 1 + unhighlight the last selected
		// the events happening in click 2
		int16 hitIndexOn196_2;
		if (clickCount == 1)
		{
			hitIndexOn196_2 = LocationToIndex(hitLocation);
			if (hitIndexOn196_2 == hitIndexOn196_1) return;
			// THERE ARE 2 POSSIBILITIES IF HITINDEXON196_2 IS NOT VALID: either 1. it's NOT VALID AT ALL, 2. it's re-selection of another piece in current player's side
			if (!isClick2MoveIndexValid(hitIndexOn196_2))
			{
				UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is not valid at %d with value %d!"), hitIndexOn196_2, IndexInPieceVectorFromBoardIndex[hitIndexOn196_2]);
				for (int i = 0; i < ActiveIndex; i++)
					UE_LOG(LogTemp, Warning, TEXT("Movable indices at %d contains index: %d!"), i, MovableIndices[i]);
				// IF IT'S THE  PLAYER CLICK AGAIN ON ONE OF HIS PIECE (we take it as re-selection) and cast it back to click1
				if (isClick1P1IndexValid(hitIndexOn196_2))	// re-selection happens	
				{
					// RE-ASSIGN ENPASSANT AS WE NEED TO RE-EVALUATE IT AGAIN
					bEnPassant = false;
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOn196_1, true);

					// AS WE RE-SELECT, WE RE-ASSIGN HITINDEX1 WITH HITINDEX2 VALUE, WAITING FOR CLICK2
					hitIndexOn196_1 = hitIndexOn196_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOn196_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
						GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				// TODO: we might have to add checkMate at this stage
				makeMove(hitIndexOn196_1, hitIndexOn196_2);
				// WHATEVER IT WAS, IF MOVED THEN EN PASSANT IS RESET
				bEnPassant = false;
				// we wanna check the checkMate from this specific Piece's NEW POSITION, this is check-mating from us to the enemy after we move

// KhanhCD Modify Start 2021-08-22
				checkMateToEnemyTeam(hitIndexOn196_2);
				MovableHashKeys.Init(-1, 196);
				checkMateToThisTeam(-1);
				MovableHashKeys.Init(-1, 196);

				if (bP1Checked || bP3Checked)
					UE_LOG(LogTemp, Warning, TEXT("Team 1 lose!"));
// KhanhCD Modify End 2021-08-22

				playerTurn = -2;
			}
		}
	}
	else if (playerTurn == -2)
	{
		// the events happening in Click1
		if (clickCount == 0)
		{
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P2IndexValid(hitIndexOn196_1)) return;

			clickCount = 1;
			const int16 hitValue1 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
			int16 startStorageIndex = 0;
			HighlightSelectedTileByIndex(hitIndexOn196_1);

			if (isPawn(hitValue1))
			{
				UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
				GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		int16 hitIndexOn196_2;
		if (clickCount == 1)
		{
			hitIndexOn196_2 = LocationToIndex(hitLocation);
			if (hitIndexOn196_2 == hitIndexOn196_1) return;

			if (!isClick2MoveIndexValid(hitIndexOn196_2))
			{
				// RE-ASSIGN ENPASSANT AS WE NEED TO RE-EVALUATE IT AGAIN
				// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
				if (isClick1P2IndexValid(hitIndexOn196_2))
				{
					bEnPassant = false;
					//DEBUG
					UE_LOG(LogTemp, Warning, TEXT("Cast back to click1 from click2"));
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOn196_1, true);
					// just assign hitIndex1 <- hitIndex2, as we re-select and see this as click1 again, waiting for click2
					hitIndexOn196_1 = hitIndexOn196_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOn196_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
						GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				makeMove(hitIndexOn196_1, hitIndexOn196_2);
				bEnPassant = false;

				// KhanhCD Modify Start 2021-08-22
				checkMateToEnemyTeam(hitIndexOn196_2);
				MovableHashKeys.Init(-1, 196);
				checkMateToThisTeam(1);
				MovableHashKeys.Init(-1, 196);

				if (bP2Checked || bP4Checked)
					UE_LOG(LogTemp, Warning, TEXT("Team 2 lose!"));
				// KhanhCD Modify End 2021-08-22

				playerTurn = 3;
			}
		}
	}
	else if (playerTurn == 3)
	{
		// the events happening in Click1
		if (clickCount == 0)
		{
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P3IndexValid(hitIndexOn196_1)) return;

			clickCount = 1;
			const int16 hitValue1 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
			int16 startStorageIndex = 0;
			HighlightSelectedTileByIndex(hitIndexOn196_1);

			if (isPawn(hitValue1))
			{
				UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
				GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		int16 hitIndexOn196_2;
		if (clickCount == 1)
		{
			hitIndexOn196_2 = LocationToIndex(hitLocation);
			if (hitIndexOn196_2 == hitIndexOn196_1) return;
			if (!isClick2MoveIndexValid(hitIndexOn196_2))
			{
				// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
				if (isClick1P3IndexValid(hitIndexOn196_2))
				{   // RE-ASSIGN ENPASSANT AS WE NEED TO RE-EVALUATE IT AGAIN
					bEnPassant = false;
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOn196_1, true);

					hitIndexOn196_1 = hitIndexOn196_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOn196_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
						GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				makeMove(hitIndexOn196_1, hitIndexOn196_2);
				bEnPassant = false;

				// KhanhCD Modify Start 2021-08-22
				checkMateToEnemyTeam(hitIndexOn196_2);
				MovableHashKeys.Init(-1, 196);
				checkMateToThisTeam(-1);
				MovableHashKeys.Init(-1, 196);

				if (bP1Checked || bP3Checked)
					UE_LOG(LogTemp, Warning, TEXT("Team 1 lose!"));
				// KhanhCD Modify End 2021-08-22

				playerTurn = -4;
			}
		}
	}
	else if (playerTurn == -4)
	{
		// the events happening in Click1
		if (clickCount == 0)
		{
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P4IndexValid(hitIndexOn196_1)) return;

			clickCount = 1;
			const int16 hitValue1 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
			int16 startStorageIndex = 0;

			HighlightSelectedTileByIndex(hitIndexOn196_1);
			if (isPawn(hitValue1))
			{
				GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		int16 hitIndexOn196_2;
		if (clickCount == 1)
		{
			hitIndexOn196_2 = LocationToIndex(hitLocation);
			if (hitIndexOn196_2 == hitIndexOn196_1) return;
			if (!isClick2MoveIndexValid(hitIndexOn196_2))
			{
				// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
				if (isClick1P4IndexValid(hitIndexOn196_2))
				{
					bEnPassant = false;
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOn196_1, true);
					// just assign hitIndex1 <- hitIndex2, as we re-select and see this as click1 again, waiting for click2
					hitIndexOn196_1 = hitIndexOn196_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = pieces[IndexInPieceVectorFromBoardIndex[hitIndexOn196_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOn196_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
						GetMovableIndicesPawn(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOn196_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				makeMove(hitIndexOn196_1, hitIndexOn196_2);
				bEnPassant = false;

				// KhanhCD Modify Start 2021-08-22
				checkMateToEnemyTeam(hitIndexOn196_2);
				MovableHashKeys.Init(-1, 196);
				checkMateToThisTeam(1);
				MovableHashKeys.Init(-1, 196);

				if (bP2Checked || bP4Checked)
					UE_LOG(LogTemp, Warning, TEXT("Team 2 lose!"));
				// KhanhCD Modify End 2021-08-22

				playerTurn = 1;
			}
		}
	}
}


// MUST GET THE CURRENT TRANSFORM OF PIECE SO THAT WE CAN ALSO COPY THE ROTATION, as pieces convert back to their Instanced Original Rotation as soon as we don't plug in current Rotator 
// TODO: IF THE PLACE TO GO TO == 0 BUT IS DOING EN PASSANT
// TODO: IF THE PLACE TO GO TO != 0 (BELONGS TO THE ENEMY)
// TODO: add the checkMate at the end of the turn 
//  TODO: check both from enemy to us and from us to the enemy
void ACustomPlayerController::makeMove(const int16& indexOn196_1, int16 indexOn196_2)
{
	const int16 pieceValue = pieces[IndexInPieceVectorFromBoardIndex[indexOn196_1]]->value;
	FVector newLocation = Index196ToLocation(indexOn196_2);

	const int16 IndexInPieceVector = IndexInPieceVectorFromBoardIndex[indexOn196_1];

	// Logic here: if the piece is type (king, rook, or pawn), then we might have to change the FirstMove
	if (isPawn(pieceValue) || isKing(pieceValue) || isRook(pieceValue))
	{
		if (pieces[IndexInPieceVector]->firstMove == 1)
			pieces[IndexInPieceVector]->firstMove = 2;
		else if (pieces[IndexInPieceVector]->firstMove == 2)
			pieces[IndexInPieceVector]->firstMove = 0;
	}

	pieces[IndexInPieceVector]->SetActorLocation(newLocation);

	// IF THE MOVE IS TO EAT
	if (IndexInPieceVectorFromBoardIndex[indexOn196_2] != -1)
	{	// NORMAL EATING CASE
		moveToGraveyard(indexOn196_2);
		// update the indexOnboard in the pieceVector
		pieces[IndexInPieceVectorFromBoardIndex[indexOn196_2]]->indexOnBoard = -1;
		// then update on the chess board for later swapping
		IndexInPieceVectorFromBoardIndex[indexOn196_2] = -1;
	}
	else
	{	// ENPASSANT EATING CASE: from analysis, en passant position can only fall within the 'cross' (4 indices) around index2
		// TODO: CURRENT IMPLEMENTATION IS CAUSING SEG FAULT FOR MOVE TO GRAVEYARD
		if (bEnPassant)
		{	// TODO: divide this into playerTurn (so that it not eat the one matching the condition first)
			int16 indexToGraveyard;
			if (playerTurn == -2 || playerTurn == -4)
			{ // for team 2 (left and right) only check top and bottom
				if (IndexInPieceVectorFromBoardIndex[indexOn196_2 + 14] > -1)	// '> -1' condition hedge against '-2' as well so it's better
					indexToGraveyard = indexOn196_2 + 14;
				else if (IndexInPieceVectorFromBoardIndex[indexOn196_2 - 14] > -1)
					indexToGraveyard = indexOn196_2 - 14;
			}
			if (playerTurn == 1 || playerTurn == 3)
			{ // for team 1 (top and bottom) only check left and right
				if (IndexInPieceVectorFromBoardIndex[indexOn196_2 - 1] > -1)
					indexToGraveyard = indexOn196_2 - 1;
				if (IndexInPieceVectorFromBoardIndex[indexOn196_2 + 1] > -1)
					indexToGraveyard = indexOn196_2 + 1;
			}
			moveToGraveyard(indexToGraveyard);
			// update the index of the eaten piece to =-1
			pieces[IndexInPieceVectorFromBoardIndex[indexToGraveyard]]->indexOnBoard = -1;
			// then update index on the chess board for correct swapping values later
			IndexInPieceVectorFromBoardIndex[indexToGraveyard] = -1;
		}
		// reset EnPassant to false again
		bEnPassant = false;
	}
	// unhighlight all the suggested Tiles
	for (int i = 0; i < ActiveIndex; i++)
		UnhighlightTileByIndex(MovableIndices[i], false);
	UnhighlightTileByIndex(indexOn196_1, true);

	// in the pieceVector, we update the indexOnBoard of the vector to new location
	pieces[IndexInPieceVectorFromBoardIndex[indexOn196_1]]->indexOnBoard = indexOn196_2;
	// then we swap the index. We already set the newLocation = -1 (and for EnPassant the eaten place = -1), so we can swap now
	IndexInPieceVectorFromBoardIndex.Swap(indexOn196_1, indexOn196_2);

}

// TODO: THIS IS CURRENT WRONG FOR EN PASSANT AS WE HAVE ALREADY PASSED THE -1 TILE, THE ENEMY TILE HAS NOT BEEN ASSIGNED TO EMPTY 
void ACustomPlayerController::moveToGraveyard(const int16& hitIndexOn196_2)
{ // TODO: MOVE the type to appropriate place (now it's all move to same place)

	const int8 IndexInPieceVector = IndexInPieceVectorFromBoardIndex[hitIndexOn196_2];
	UE_LOG(LogTemp, Warning, TEXT("IndexInPieceVector: %d"), IndexInPieceVector);
	const int8 pieceValue = pieces[IndexInPieceVector]->value;

	// TODO: there will be separate places for each type of piece, not currently implemented
	if (isPawn(pieceValue))
		pieces[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isRook(pieceValue))
		pieces[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isKnight(pieceValue))
		pieces[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isBishop(pieceValue))
		pieces[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isQueen(pieceValue))
		pieces[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isKing(pieceValue))
		pieces[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
}

/* EXTRA FEATURES */
// TODO: CREATE INSTANCED MESH COMPONENTS, OR SOMETHING THAT WE CAN CHANGE FROM x3 to x4 etc..
// we can place before hand with visibility = 0 then once there's a shit eaten we can ..

// We will have materials at FIXED indices that serve the following function:
// [0]. Coloring Tile type1 (depends on Tile)
// [1]. Coloring Tile type2 (depends on Tile)
// [2]. Pure highlight (default = 0.f)
// TODO: create the highlight color on another Lerp so that we can toggle on or off on a specific channel using specific DataValue
void ACustomPlayerController::HighlightSelectedTileByLocation(const FVector& hitLocation)
{
	if (!Board)
		UE_LOG(LogTemp, Warning, TEXT("The board is NUL"));
	const FVector2D XYIndex = LocationToXY(hitLocation);
	const int16 indexToHighlight = XYToIndex(XYIndex.X, XYIndex.Y);
	UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

void ACustomPlayerController::HighlightSelectedTileByIndex(const int16& indexToHighlight)
{
	if (!Board)
		UE_LOG(LogTemp, Warning, TEXT("The board is NULL"));
	UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

// TODO: THIS IS NOT FINISHED, the color numeric values are just place holders
// Board is still ISM, thus we still keep this code
void ACustomPlayerController::HighlightMovableIndices(const int16& startMovableIndex = 0)	// by default we pass 0 if no value is passed in
{
	if (ActiveIndex != 0)
	{
		// highlight all movable indices
		int16 i = startMovableIndex;
		int16 index196;
		UE_LOG(LogTemp, Warning, TEXT("Active index at HMI: %d"), ActiveIndex);
		for (/*  */; i < ActiveIndex - 1; i++)
		{
			index196 = MovableIndices[i];
			if (IndexInPieceVectorFromBoardIndex[index196] == -1)
				// Normal Highlight (blank tile)
				Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, false);
			else
			{
				if (bEnPassant)
				{   // highlight RED at enPassant eatable place
					if (IndexInPieceVectorFromBoardIndex[index196 + 14] > -1)
						Board->InstancedStaticMeshComponent->SetCustomDataValue(index196 + 14, 0, 1.f, false);
					else if (IndexInPieceVectorFromBoardIndex[index196 - 14] > -1)
						Board->InstancedStaticMeshComponent->SetCustomDataValue(index196 - 14, 0, 1.f, false);
					else if (IndexInPieceVectorFromBoardIndex[index196 - 1] > -1)
						Board->InstancedStaticMeshComponent->SetCustomDataValue(index196 - 1, 0, 1.f, false);
					else
						Board->InstancedStaticMeshComponent->SetCustomDataValue(index196 + 1, 0, 1.f, false);
				}
				else
				{	// highlight RED at normal eatable places
					Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, false);
				}
			}
		}

		// we want to MarkRenderDirty only at last of loop (bcoz it's costly), thus separate
		// this i is the 'failed condition' i, which means it has already incremented to activeIndex - 1 (the last index)
		index196 = MovableIndices[i];
		UE_LOG(LogTemp, Warning, TEXT("last movable Index is: %d"), index196);
		// same check logic
		if (IndexInPieceVectorFromBoardIndex[index196] == -1)
			Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, true);
		else
			Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, true);
	}
}

void ACustomPlayerController::UnhighlightTileByIndex(const int16& index, const bool& bMarkRenderStateDirty = false)
{// change board material back to its original state
 // TODO: do a correct implementation of this after change the M_material in Unreal Editor
	Board->InstancedStaticMeshComponent->SetCustomDataValue(index, 0, 0.f, bMarkRenderStateDirty);
}

FVector2D ACustomPlayerController::LocationToXY(const FVector& hitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Location2Index: Receive Location: %s"), *hitLocation.ToString());
	return FVector2D(hitLocation.X, hitLocation.Y);
}

int16 ACustomPlayerController::XYToIndex(const int16& XValue, const int16& YValue)
{
	return XValue / 400 * 14 + YValue / 400;
}

FVector ACustomPlayerController::Index196ToLocation(const int16& index)
{
	int16 X = index / 14 * 400 + 200;
	int16 Y = index % 14 * 400 + 200;
	return FVector(X, Y, 0.f);
}

// TODO: CREATE UE_LOG FOR ANY TEAM THAT'S CHECKMATED (AND CHECKMATING OTHER TEAM? maybe?) SUCCEED FOR THE PURPOSE OF DEBUGGING
// as we only call this check after successfully makeMove() to index196_2, each getMovableIndices() function is sure to be valid (as makeMove() only successfully executed if the 2nd (or move-to) index is valid)
void ACustomPlayerController::checkMateToEnemyTeam(const int16& boardIndex)
{
	const int16 kingBottomPosition = pieces[15]->indexOnBoard;
	const int16 kingTopPosition = pieces[31]->indexOnBoard;
	const int16 kingLeftPosition = pieces[47]->indexOnBoard;
	const int16 kingRightPosition = pieces[63]->indexOnBoard;

	const int16 indexInPieceVector = IndexInPieceVectorFromBoardIndex[boardIndex];

	/// because this check mate check from our team to the enemy team, our team is the attacker, thus we pass the 'playerTurn' as the attacker sign
	if (isPawn(pieces[indexInPieceVector]->value))
		GetMovableIndicesPawn(ActiveIndex, boardIndex, true, playerTurn);
	else if (isRook(pieces[indexInPieceVector]->value))
		GetMovableIndicesRook(ActiveIndex, boardIndex, true, playerTurn);
	else if (isKnight(pieces[indexInPieceVector]->value))
		GetMovableIndicesKnight(ActiveIndex, boardIndex, true, playerTurn);
	else if (isBishop(pieces[indexInPieceVector]->value))
		GetMovableIndicesBishop(ActiveIndex, boardIndex, true, playerTurn);
	else if (isKing(pieces[indexInPieceVector]->value))
		GetMovableIndicesKing(ActiveIndex, boardIndex, true, playerTurn);
	else if (isQueen(pieces[indexInPieceVector]->value))
		GetMovableIndicesQueen(ActiveIndex, boardIndex, true, playerTurn);

	if (playerTurn == 1 || playerTurn == 3)
	{
		if (MovableHashKeys[kingLeftPosition] == kingLeftPosition)
			bP2Checked = true;
		if (MovableHashKeys[kingRightPosition] == kingRightPosition)
			bP4Checked = true;
	}
	else if (playerTurn == -2 || playerTurn == -4)
	{
		if (MovableHashKeys[kingBottomPosition] == kingBottomPosition)
			bP1Checked = true;
		if (MovableHashKeys[kingTopPosition] == kingTopPosition)
			bP3Checked = true;
	}
}

// TODO: fix this function to work NULL SAFE (i.e. work even when CURRENT INDEX NOT VALID (i.e WORK EVEN WHEN EATEN PIECES indices BEING PASSED IN, BY SKIPPING THEM))
// TODO: AND ALSO FIX THAT ERROR IN OTHER SIMILAR FUNCTIONS AS WELL
// since it's checkMate to CURRENT TEAM (check if current TEam is being checked mate), the attackers are the opposite team, thus their sign are opposite
void ACustomPlayerController::checkMateToThisTeam(const int16& attackerPlayerSign)
{
	if (playerTurn == 1 || playerTurn == 3)
	{   // get all movables of team 2 & team 4 into hashKeys
		const int P1KingPosition = pieces[15]->indexOnBoard;
		const int P3KingPosition = pieces[31]->indexOnBoard;
		// team 2
		// The attackPlayerSign is for used in any eating move that uses '* attackPlayerSign' to check the validity of the eat index 
		for (int i = 32; i <= 39; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 40; i <= 41; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 42; i <= 43; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 44; i <= 45; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		if (pieces[46]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, pieces[46]->indexOnBoard, true, attackerPlayerSign);
		if (pieces[47]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, pieces[47]->indexOnBoard, true, attackerPlayerSign);

		// team 4
		for (int i = 48; i <= 55; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 56; i <= 57; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 58; i <= 59; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 60; i <= 61; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		if (pieces[62]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, pieces[62]->indexOnBoard, true, attackerPlayerSign);
		if (pieces[63]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, pieces[63]->indexOnBoard, true, attackerPlayerSign);
		if (MovableHashKeys[P1KingPosition] == P1KingPosition)
		{	// if it contains King Movable then we lose
			bP1Checked = true;
		}
		else
		{   // else if we are already being checked, but after our move it no longer contains the King position -> change the status of being checked to false
			if (bP1Checked)
				bP1Checked = false;
		}
		// SAME LOGIC APPLIES HERE
		if (MovableHashKeys[P3KingPosition] == P3KingPosition)
		{
			bP3Checked = true;
		}
		else
		{
			if (bP3Checked)
				bP3Checked = false;
		}
	}

	if (playerTurn == -2 || playerTurn == -4)
	{   // get all movables of team 1 & team 3 into hashKeys
		const int P2KingPosition = pieces[47]->indexOnBoard;
		const int P4KingPosition = pieces[63]->indexOnBoard;
		// team 1
		// The attackPlayerSign is for used in any eating move that uses '* attackPlayerSign' to check the validity of the eat index 
		for (int i = 0; i <= 7; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 8; i <= 9; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 10; i <= 11; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 12; i <= 13; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		if (pieces[14]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, pieces[14]->indexOnBoard, true, attackerPlayerSign);
		if (pieces[15]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, pieces[15]->indexOnBoard, true, attackerPlayerSign);

		// team 3
		for (int i = 16; i <= 23; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 24; i <= 25; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 26; i <= 27; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		for (int i = 28; i <= 29; i++)
		{
			if (pieces[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, pieces[i]->indexOnBoard, true, attackerPlayerSign);
		}
		if (pieces[30]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, pieces[30]->indexOnBoard, true, attackerPlayerSign);
		if (pieces[31]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, pieces[31]->indexOnBoard, true, attackerPlayerSign);
		if (MovableHashKeys[P2KingPosition] == P2KingPosition)
		{	// if it contains King Movable then we lose
			bP2Checked = true;
		}
		else
		{   // else if we are already being checked, but after our move it no longer contains the King position -> change the status of being checked to false
			if (bP2Checked)
				bP2Checked = false;
		}
		// SAME LOGIC APPLIES HERE
		if (MovableHashKeys[P4KingPosition] == P4KingPosition)
		{
			bP4Checked = true;
		}
		else
		{
			if (bP4Checked)
				bP4Checked = false;
		}
	}
}

int16 ACustomPlayerController::LocationToIndex(const FVector& hitLocation)
{
	return FMath::Floor(hitLocation.X / 400) * 14 + FMath::Floor(hitLocation.Y / 400);
}
/* TODO: Lam function NhapThanh() */
// TODO: NOW, CASTLING SHOULD BE CONSIDERED AS A MOVABLES OF THE KING, UNTIL THE KING IS NO LONGER APPROPRIATE FOR THAT MOVE

// TODO: MAKE ENPASSANT HAPPENS ONLY AT 4 CORNERS OF THE TABLE
void ACustomPlayerController::GetMovableIndicesPawn(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& dummyVariable)
{ // TODO: Segfault error
	UE_LOG(LogTemp, Warning, TEXT("GetMovablesPawn is triggered!"));
	ActiveIndex = startStorageIndex;
	bEnPassant = false;

	// TODO: for each team the Pawn will be moving in different axes, make sure to do them after you've done networking
	// for bottom team - POSITIVE
	if (playerTurn == 1)
	{
		/* MOVE TO MOVE MOVE: STRAIGHT FORWARD */
		if (currentIndex + 14 < 196)
		{
			if (IndexInPieceVectorFromBoardIndex[currentIndex + 14] != -2)
			{
				if (IndexInPieceVectorFromBoardIndex[currentIndex + 14] == -1)
				{
					UE_LOG(LogTemp, Warning, TEXT("1 move ahead is fired!"));
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex + 14;
					else
						MovableHashKeys[currentIndex + 14] = currentIndex + 14;
				}
				// EN PASSANT: 
				//  TODO: EN PASSANT IS CURRENTLY CORRECT IN DETECTING THE CELL, BUT IS NOT PERFORMING THE EAT (NOT CURRENTLY DESTROYING THE ENEMY PIECE)
				// Should we create a bEnPassant, which will always be assigned false at the end of MakeMove() and in this stage will be used to detect whether an enPassant should be ritualed?
				// IF IT'S RELATED TO VALID INDEXES AROUND ENEMY FIRST MOVE THEN IT'S GUARANTEED THAT THE INDEX IS VALID
				// TODO: SAFE CHECK HERE TO MAKE SURE INDEX IN RANGE
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 14]]->value == -1)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 14]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex + 13;
						else
							MovableHashKeys[currentIndex + 13] = currentIndex + 13;
					}
					bEnPassant = true;
				}
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 14]]->value == -11)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 14]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex + 15;
						else
							MovableHashKeys[currentIndex + 15] = currentIndex + 15;
					}
					bEnPassant = true;
				}
			}
		}

		/*  THE MOVE TO EAT (thus we don't consider == 0) */
		// since we are doing demo for positive team, if value[position] > 0 then it's enemy
		// since this is C++, we always have to do an check valid index first
		// TODO: check if we have to do a Tight Condition check (simulate if Pawn keeps going diagonal from the starting point all the way and see if it could reach invalid indices/places)
		// TODO:  YES WE DO, AS THE PAWN NEAR THE CORNER EASILY FALLS OFF THE THRESHOLD

		// left and right threshold are NON-STRICT
		// both these eats are on the same row as moving so we can use either of eatRight or eatLeft as argument
		int16 eatRight = currentIndex + 15;
		int16 eatLeft = currentIndex + 13;
		int16 rightThreshold = RightThresholdOfIndex(eatRight);
		int16 leftThreshold = LeftThresholdOfIndex(eatRight);

		if (eatRight <= rightThreshold)
		{
			// we are seeing the move the eat so the value should not be -1 or -2, which means it's >= 0
			if (IndexInPieceVectorFromBoardIndex[eatRight] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatRight]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatRight;
					else
						MovableHashKeys[eatRight] = eatRight;
				}
			}
		}
		if (eatLeft >= leftThreshold)
		{
			if (IndexInPieceVectorFromBoardIndex[eatLeft] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatLeft]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatLeft;
					else
						MovableHashKeys[eatLeft] = eatLeft;
				}
			}
		}
	}
	// TODO: write movable hash keys for 3 other players 
	else if (playerTurn == -2)
	{ // for left team - NEGATIVE
		const int16 moveRightThreshold = RightThresholdOfIndex(currentIndex);
		/* THE MOVE TO MOVE - STRAIGHT RIGHT */
		if (currentIndex + 1 < moveRightThreshold)
		{
			if (IndexInPieceVectorFromBoardIndex[currentIndex + 1] != -2)
			{
				if (IndexInPieceVectorFromBoardIndex[currentIndex + 1] == -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex + 1;
					else
						MovableHashKeys[currentIndex + 1] = currentIndex + 1;
				}
				// EN PASSANT:
				// IF IT'S RELATED TO VALID INDEXES AROUND ENEMY FIRST MOVE THEN IT'S GUARANTEED THAT THE INDEX IS VALID
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 1]]->value == 1)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 1]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex - 13;
						else
							MovableHashKeys[currentIndex - 13] = currentIndex - 13;
					}
					bEnPassant = true;
				}
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 1]]->value == 11)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex + 1]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex + 15;
						else
							MovableHashKeys[currentIndex + 15] = currentIndex + 15;
					}
					bEnPassant = true;
				}
			}
		}
		/*  THE MOVE TO EAT (thus we don't consider == 0) */
		const int16 eatUp = currentIndex + 15;
		const int16 eatDown = currentIndex - 13;
		const int16 eatUpRightThreshold = RightThresholdOfIndex(eatUp);
		const int16 eatDownRightThreshold = RightThresholdOfIndex(eatDown);

		if (eatUp <= eatUpRightThreshold && eatUp < 196)
		{
			if (IndexInPieceVectorFromBoardIndex[eatUp] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatUp]]->value > 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatUp;
					else
						MovableHashKeys[eatUp] = eatUp;
				}
			}
		}
		// since there's no eating backwards so we don't have to compare with leftThreshold
		if (eatDown <= eatDownRightThreshold && eatDown >= 0)
		{
			if (IndexInPieceVectorFromBoardIndex[eatDown] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatDown]]->value > 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatDown;
					else
						MovableHashKeys[eatDown] = eatDown;
				}
			}
		}
	}

	else if (playerTurn == 3)
	{ // for top team - POSITIVE
		/* THE MOVE TO MOVE + EN PASSANT */
		// TODO: LATER IF WE WANT TO ADD MOVING BACK FROM FRIEND TEAM, THEN WE HAVE TO USE TIGHT CONDITION
		const int16 oneRowDownLeftThreshold = LeftThresholdOfIndex(currentIndex) - 14;
		const int16 oneRowDownRightThreshold = oneRowDownLeftThreshold + 13;
		if (currentIndex - 14 >= oneRowDownLeftThreshold && currentIndex - 14 <= oneRowDownRightThreshold)
		{
			if (IndexInPieceVectorFromBoardIndex[currentIndex-14] != -2)
			{
				/* THE MOVE TO MOVE */
				if (IndexInPieceVectorFromBoardIndex[currentIndex - 14] == -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex - 14;
					else
						MovableHashKeys[currentIndex - 14] = currentIndex - 14;
				}
				// EN PASSANT:
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 14]]->value == -1)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 14]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex - 15;
						else
							MovableHashKeys[currentIndex - 15] = currentIndex - 15;
					}
					bEnPassant = true;
				}
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 14]]->value == -11)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 14]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex - 13;
						else
							MovableHashKeys[currentIndex - 13] = currentIndex - 13;
					}
					bEnPassant = true;
				}
			}
		}
		/*  THE MOVE TO EAT (thus we don't consider == 0) */
		// since we are doing demo for positive team, if value[position] > 0 then it's enemy
		// since this is C++, we always have to do an check valid index first
		// TODO: check if we have to do a Tight Condition check (simulate if Pawn keeps going diagonal from the starting point all the way and see if it could reach invalid indices/places)
		const int16 eatLeft = currentIndex - 15;
		const int16 eatRight = currentIndex - 13;
		const int16 eatLeftThreshold = LeftThresholdOfIndex(eatLeft);
		const int16 eatRightThreshold = RightThresholdOfIndex(eatRight);
		if (eatRight <= eatRightThreshold && eatRight >= 0)
		{
			if (IndexInPieceVectorFromBoardIndex[eatRight] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatRight]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatRight;
					else
						MovableHashKeys[eatRight] = eatRight;
				}
			}
		}
		if (eatLeft >= eatLeftThreshold)
		{
			if (IndexInPieceVectorFromBoardIndex[eatLeft] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatLeft]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatLeft;
					else
						MovableHashKeys[eatLeft] = eatLeft;
				}
			}
		}
	}

	else if (playerTurn == -4)
	{ // for right team - NEGATIVE
		const int16 moveLeftThreshold = LeftThresholdOfIndex(currentIndex);
		/* THE MOVE TO MOVE */
		if (currentIndex - 1 > moveLeftThreshold)
		{
			if (IndexInPieceVectorFromBoardIndex[currentIndex - 1] != -2)
			{
				if (IndexInPieceVectorFromBoardIndex[currentIndex - 1] == -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex - 1;
					else
						MovableHashKeys[currentIndex - 1] = currentIndex - 1;
				}
				// EN PASSANT: ANALYZE ENEMY PAWN FIRST MOVE STATUS
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 1]]->value == 1)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 1]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex - 15;
						else
							MovableHashKeys[currentIndex - 15] = currentIndex - 15;
					}
					bEnPassant = true;
				}
				else if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 1]]->value == 11)
				{
					if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex - 1]]->firstMove == 2)
					{
						if (bUseHash == false)
							MovableIndices[ActiveIndex++] = currentIndex + 13;
						else
							MovableHashKeys[currentIndex + 13] = currentIndex + 13;
					}
					bEnPassant = true;
				}
			}
		}
		/*  THE MOVE TO EAT ONLY (thus we don't consider == -1) */
		// since we are doing demo for positive team, if value[position] > 0 then it's enemy
		// since this is C++, we always have to do an check valid index first
		// TODO: check if we have to do a Tight Condition check (simulate if Pawn keeps going diagonal from the starting point all the way and see if it could reach invalid indices/places)
		const int16 eatUp = currentIndex + 13;
		const int16 eatDown = currentIndex - 15;
		const int16 eatUpLeftThreshold = LeftThresholdOfIndex(eatUp);
		const int16 eatDownLeftThreshold = LeftThresholdOfIndex(eatDown);

		if (eatUp >= eatUpLeftThreshold && eatUpLeftThreshold < 196)
		{
			if (IndexInPieceVectorFromBoardIndex[eatUp] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatUp]]->value > 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatUp;
					else
						MovableHashKeys[eatUp] = eatUp;
				}
			}
		}
		if (eatDown >= 0 && eatDown >= eatDownLeftThreshold)
		{
			if (IndexInPieceVectorFromBoardIndex[eatDown] >= 0)
			{
				if (pieces[IndexInPieceVectorFromBoardIndex[eatDown]]->value > 0 && IndexInPieceVectorFromBoardIndex[eatDown] != -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatDown;
					else
						MovableHashKeys[eatDown] = eatDown;
				}
			}
		}
	}

	/* FIRST MOVE SPECIALS */
	// if the current piece having first move then it's guaranteed to be valid, so we don't have to validate with condition check again
	if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex]]->firstMove == 1) // FIRST MOVE
	{
		if (playerTurn == 1)
		{ // FOR BOTTOM TEAM ONLY
			if (IndexInPieceVectorFromBoardIndex[currentIndex + 14] == -1 && IndexInPieceVectorFromBoardIndex[currentIndex + 28] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = currentIndex + 28;
				else
					MovableHashKeys[currentIndex + 28] = currentIndex + 28;
			}
		}
		else if (playerTurn == -2)
		{ // FOR LEFT TEAM ONLY
			if (IndexInPieceVectorFromBoardIndex[currentIndex + 1] == -1 && IndexInPieceVectorFromBoardIndex[currentIndex + 2] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = currentIndex + 2;
				else
					MovableHashKeys[currentIndex + 2] = currentIndex + 2;
			}
		}
		else if (playerTurn == 3)
		{ // FOR LEFT TEAM ONLY
			if (IndexInPieceVectorFromBoardIndex[currentIndex - 14] == -1 && IndexInPieceVectorFromBoardIndex[currentIndex - 28] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = currentIndex - 28;
				else
					MovableHashKeys[currentIndex - 28] = currentIndex - 28;
			}
		}
		else if (playerTurn == -4)
		{ // FOR LEFT TEAM ONLY
			if (IndexInPieceVectorFromBoardIndex[currentIndex - 1] == -1 && IndexInPieceVectorFromBoardIndex[currentIndex - 2] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = currentIndex - 2;
				else
					MovableHashKeys[currentIndex - 2] = currentIndex - 2;
			}
		}
	}
	//for (int i = 0; i < ActiveIndex; i++)
	//	UE_LOG(LogTemp, Warning, TEXT("Movable index: %d"), MovableIndices[i]);
}

// TODO: Fix this * attackerPlayerSign as its logic is error with CheckMateToThisTeam (check other team movables on our Turn)
void ACustomPlayerController::GetMovableIndicesRook(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign)
{
	/*
		 Moving Logic as follow:
		 + VERTICALLY: going up 1 row: = current Index +14 or Down 1 row: = currentIndex - 14
		 + HORIZONTALLY:
		   - Horizontal left threshold = last index of previous row
		   - Horizontal right threshold = first index of next row
	*/
	// the last index of the row below it
	const int16 LeftThreshold = FMath::Floor(currentIndex / 14) * 14;
	// the first index of the row above it
	const int16 RightThreshold = LeftThreshold + 13;

	// assign all with the current index of the chess piece
	int16 goLeft = currentIndex - 1,
		goRight = currentIndex + 1,
		goUp = currentIndex + 14,
		goDown = currentIndex - 14;

	ActiveIndex = startStorageIndex;
	// CALCULATE HORIZONTAL MovableIndices OF TEAMs (base on whether current Player IS TEAM NEGATIVE or POSITIVE) 
	while (goLeft >= LeftThreshold && IndexInPieceVectorFromBoardIndex[goLeft] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goLeft] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goLeft;
			else
				MovableHashKeys[goLeft] = goLeft;
			goLeft--;
		}
		else
		{
			// if reach one of our side then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goLeft]]->value * attackerPlayerSign > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goLeft;
				else
					MovableHashKeys[goLeft] = goLeft;
				break;
			}
		}
	}

	while (goRight <= RightThreshold && IndexInPieceVectorFromBoardIndex[goRight] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goRight] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goRight;
			else
				MovableHashKeys[goRight] = goRight;
			goRight++;
		}
		else
		{
			// if reach one of our side then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goRight]]->value * attackerPlayerSign > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goRight;
				else
					MovableHashKeys[goRight] = goRight;
				break;
			}
		}
	}

	while (goUp <= 195 && IndexInPieceVectorFromBoardIndex[goUp] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goUp] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goUp;
			else
				MovableHashKeys[goUp] = goUp;
			goUp += 14;
		}
		else
		{
			// if reach one of our side then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goUp]]->value * attackerPlayerSign > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goUp]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goUp;
				else
					MovableHashKeys[goUp] = goUp;
				break;
			}
		}
	}

	while (goDown >= 0 && IndexInPieceVectorFromBoardIndex[goDown] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goDown] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goDown;
			else
				MovableHashKeys[goDown] = goDown;
			goDown -= 14;
		}
		else
		{ 
			// if reach one of our side then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goDown]]->value * attackerPlayerSign > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (pieces[IndexInPieceVectorFromBoardIndex[goDown]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goDown;
				else
					MovableHashKeys[goDown] = goDown;
				break;
			}
		}
	}
}

// TODO: Check all functions for SIGSEV safe with this new implentation (index != -1 and -2 before passing into pieces)
void ACustomPlayerController::GetMovableIndicesKnight(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign)
{
	int16 L2RowUpRight = currentIndex + 29,
		L1RowUpRight = currentIndex + 16,
		L2RowUpLeft = currentIndex + 27,
		L1RowUpLeft = currentIndex + 12,
		L2RowDownRight = currentIndex - 27,
		L1RowDownRight = currentIndex - 12,
		L2RowDownLeft = currentIndex - 29,
		L1RowDownLeft = currentIndex - 16;

	ActiveIndex = startStorageIndex;

	// for bottom player
	//TODO: have to compare these to the minimum of the board and maximum of the board
	// TODO: we can just put that right into the threshold calculator
	// TODO: CHANGE ALL STAND AND LIE EQUATION TO BEING RELATIVE OF OTHERS BEFORE IT, instead of heavily calling the function
	const int16 currentRowLeftThreshold = LeftThresholdOfIndex(currentIndex);
	const int16 currentRowRightThreshold = RightThresholdOfIndex(currentIndex);

	// we don't have to use std::min() here because later we will independently check for that condition later
	// TODO: consider merging the condition?
	const int16 oneRowUpLeftThreshold = currentRowLeftThreshold + 14;
	const int16 oneRowUpRightThreshold = currentRowRightThreshold + 14;
	const int16 twoRowUpLeftThreshold = currentRowLeftThreshold + 28;
	const int16 twoRowUpRightThreshold = currentRowRightThreshold + 28;
	const int16 oneRowDownLeftThreshold = currentRowLeftThreshold - 14;
	const int16 oneRowDownRightThreshold = currentRowRightThreshold - 14;
	const int16 twoRowDownLeftThreshold = currentRowLeftThreshold - 28;
	const int16 twoRowDownRightThreshold = currentRowRightThreshold - 28;

	UE_LOG(LogTemp, Warning, TEXT("current Row left thres: %d"), currentRowLeftThreshold);
	UE_LOG(LogTemp, Warning, TEXT("current Row right thres: %d"), currentRowRightThreshold);
	UE_LOG(LogTemp, Warning, TEXT("L1RowDownRight: %d"), L1RowDownRight);
	UE_LOG(LogTemp, Warning, TEXT("L1RowUpRight: %d"), L1RowUpRight);
	UE_LOG(LogTemp, Warning, TEXT("oneRowUpRight Threshold: %d"), oneRowUpRightThreshold);
	UE_LOG(LogTemp, Warning, TEXT("oneRowUpLeft Threshold: %d"), oneRowUpLeftThreshold);

	// all are non-strict condition
	if (twoRowUpRightThreshold <= 195 && L2RowUpRight <= twoRowUpRightThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L2RowUpRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L2RowUpRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpRight;
				else
					MovableHashKeys[L2RowUpRight] = L2RowUpRight;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L2RowUpRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpRight;
				else
					MovableHashKeys[L2RowUpRight] = L2RowUpRight;
			}
		}
	}

	if (twoRowUpLeftThreshold <= 195 && L2RowUpLeft >= twoRowUpLeftThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L2RowUpLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L2RowUpLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpLeft;
				else
					MovableHashKeys[L2RowUpLeft] = L2RowUpLeft;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L2RowUpLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpLeft;
				else
					MovableHashKeys[L2RowUpLeft] = L2RowUpLeft;
			}
		}
	}

	if (oneRowUpRightThreshold <= 195 && L1RowUpRight <= oneRowUpRightThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L1RowUpRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L1RowUpRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpRight;
				else
					MovableHashKeys[L1RowUpRight] = L1RowUpRight;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L1RowUpRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpRight;
				else
					MovableHashKeys[L1RowUpRight] = L1RowUpRight;
			}
		}
	}

	if (oneRowUpLeftThreshold <= 195 && L1RowUpLeft >= oneRowUpLeftThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L1RowUpLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L1RowUpLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpLeft;
				else
					MovableHashKeys[L1RowUpLeft] = L1RowUpLeft;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L1RowUpLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpLeft;
				else
					MovableHashKeys[L1RowUpLeft] = L1RowUpLeft;
			}
		}
	}
	/* DOWNWARDS */
	if (L2RowDownRight >= 0 && L2RowDownRight <= twoRowDownRightThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L2RowDownRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L2RowDownRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownRight;
				else
					MovableHashKeys[L2RowDownRight] = L2RowDownRight;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L2RowDownRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownRight;
				else
					MovableHashKeys[L2RowDownRight] = L2RowDownRight;
			}
		}
	}
	if (twoRowDownLeftThreshold >= 0 && L2RowDownLeft >= twoRowDownLeftThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L2RowDownLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L2RowDownLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownLeft;
				else
					MovableHashKeys[L2RowDownLeft] = L2RowDownLeft;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L2RowDownLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownLeft;
				else
					MovableHashKeys[L2RowDownLeft] = L2RowDownLeft;
			}
		}
	}
	if (L1RowDownRight >= 0 && L1RowDownRight <= oneRowDownRightThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L1RowDownRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L1RowDownRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownRight;
				else
					MovableHashKeys[L1RowDownRight] = L1RowDownRight;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L1RowDownRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownRight;
				else
					MovableHashKeys[L1RowDownRight] = L1RowDownRight;
			}
		}
	}

	if (oneRowDownLeftThreshold >= 0 && L1RowDownLeft >= oneRowDownLeftThreshold)
	{
		if (IndexInPieceVectorFromBoardIndex[L1RowDownLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[L1RowDownLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownLeft;
				else
					MovableHashKeys[L1RowDownLeft] = L1RowDownLeft;
			}
			else if (pieces[IndexInPieceVectorFromBoardIndex[L1RowDownLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownLeft;
				else
					MovableHashKeys[L1RowDownLeft] = L1RowDownLeft;
			}
		}
	}

	for (int i = 0; i < ActiveIndex; i++)
		UE_LOG(LogTemp, Warning, TEXT("Indices for knights: %d"), MovableIndices[i]);

}

// TODO: FINISH TEAM EATING MOVE 
void ACustomPlayerController::GetMovableIndicesBishop(int16& storageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign)
{
	int16 goUpLeft = currentIndex + 13,
		goUpRight = currentIndex + 15,
		goDownLeft = currentIndex - 15,
		goDownRight = currentIndex - 13;

	ActiveIndex = storageIndex;

	// TODO: CREATE HORIZONTAL AND VERTICAL THRESHOLD FOR THE PIECE (TIGHT CONDITION)
	//In here we've already got the vertical threshold
	// for POSTIVE TEAM
	while (goUpLeft <= 195 && IndexInPieceVectorFromBoardIndex[goUpLeft] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goUpLeft] == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("goUpLeft: Index to Add: %d"), goUpLeft);
			// TODO: APPLY THIS 'USE HASH' FOR ALL THE GET MOVABLES FUNCTIONS BELOW
			if (bUseHash == false)
				// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
				MovableIndices[ActiveIndex++] = goUpLeft;
			else
				MovableHashKeys[goUpLeft] = goUpLeft;

			goUpLeft = goUpLeft + 13;
		}
		else
		{
			// TODO: we get one more index of the enemy team into movables and ONLY AFTERWARDS do we break
			if (pieces[IndexInPieceVectorFromBoardIndex[goUpLeft]]->value * attackerPlayerSign > 0)
				break;
			if (pieces[IndexInPieceVectorFromBoardIndex[goUpLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
					MovableIndices[ActiveIndex++] = goUpLeft;
				else
					MovableHashKeys[goUpLeft] = goUpLeft;
				break;
			}
		}
	}

	while (goUpRight <= 195 && IndexInPieceVectorFromBoardIndex[goUpRight] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goUpRight] == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("goUpRight: Index to Add: %d"), goUpRight);
			// go up and shift left 1 unit relatively to current position (which already shift left 1 unit compared to the position before it - meaning that total deviation compared to original position already increases)
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goUpRight;
			else
				MovableHashKeys[goUpRight] = goUpRight;
			goUpRight = goUpRight + 15;
		}
		else
		{
			// TODO: we get one more index of the enemy team into movables and ONLY AFTERWARDS do we break
			if (pieces[IndexInPieceVectorFromBoardIndex[goUpRight]]->value * attackerPlayerSign > 0)
				break;
			if (pieces[IndexInPieceVectorFromBoardIndex[goUpRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
					MovableIndices[ActiveIndex++] = goUpRight;
				else
					MovableHashKeys[goUpRight] = goUpRight;
				break;
			}
		}
	}

	while (goDownLeft >= 0 && IndexInPieceVectorFromBoardIndex[goDownLeft] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goDownLeft] == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("goDownLeft: Index to Add: %d"), goDownLeft);
			// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goDownLeft;
			else
				MovableHashKeys[goDownLeft] = goDownLeft;
			goDownLeft = goDownLeft - 15;
		}
		else
		{
			if (pieces[IndexInPieceVectorFromBoardIndex[goDownLeft]]->value * attackerPlayerSign > 0)
				break;
			if (pieces[IndexInPieceVectorFromBoardIndex[goDownLeft]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
					MovableIndices[ActiveIndex++] = goDownLeft;
				else
					MovableHashKeys[goDownLeft] = goDownLeft;
				break;
			}
		}
	}

	while (goDownRight >= 0 && IndexInPieceVectorFromBoardIndex[goDownRight] != -2)
	{
		if (IndexInPieceVectorFromBoardIndex[goDownRight] == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("goDownRight: Index to Add: %d"), goDownRight);
			// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goDownRight;
			else
				MovableHashKeys[goDownRight] = goDownRight;
			goDownRight = goDownRight - 13;
		}
		else
		{
			if (pieces[IndexInPieceVectorFromBoardIndex[goDownRight]]->value * attackerPlayerSign > 0)
				break;
			if (pieces[IndexInPieceVectorFromBoardIndex[goDownRight]]->value * attackerPlayerSign < 0)
			{
				if (bUseHash == false)
					// shift Right 1 unit relatively to current position (which already shift Right 1 unit compared to position before it)
					MovableIndices[ActiveIndex++] = goDownRight;
				else
					MovableHashKeys[goDownRight] = goDownRight;
				break;
			}
		}
	}
}

// TODO: THIS IS CAUSING SEG FAULT
void ACustomPlayerController::GetMovableIndicesQueen(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign)
{
	GetMovableIndicesBishop(startStorageIndex, currentIndex, bUseHash, attackerPlayerSign);
	GetMovableIndicesRook(ActiveIndex, currentIndex, bUseHash, attackerPlayerSign);
}

/* TODO: THIS ONE NOT HAVING TIGHT THRESHOLD,  MIGHT CAUSE IT SEG FAULT, CHAINING THE SEGFAULT TO CHECKMATE? */
void ACustomPlayerController::GetMovableIndicesKing(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerPlayerSign)
{
	// VERTICAL AND HORIZONTAL MOVES
	const int16 kingUp = currentIndex + 14,
		kingUpLeft = kingUp - 1,
		kingUpRight = kingUp + 1,
		kingLeft = currentIndex - 1,
		kingRight = currentIndex + 1,
		kingDown = currentIndex - 14,
		kingDownLeft = kingDown - 1,
		kingDownRight = kingDown + 1;

	const int16 currentRowLeftThreshold = LeftThresholdOfIndex(currentIndex);
	const int16 currentRowRightThreshold = RightThresholdOfIndex(currentIndex);

	const int16 OneRowUpLeftThreshold = currentRowLeftThreshold + 14;
	const int16 OneRowUpRightThreshold = currentRowRightThreshold + 14;
	const int16 OneRowDownLeftThreshold = currentRowLeftThreshold - 14;
	const int16 OneRowDownRightThreshold = currentRowRightThreshold - 14;

	// DIAGONAL MOVES
	ActiveIndex = startStorageIndex;
	if (bUseHash == false)
	{
		if (kingUpLeft <= 195 && kingUpLeft >= OneRowUpLeftThreshold && IndexInPieceVectorFromBoardIndex[kingUpLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingUpLeft] == -1)
				MovableIndices[ActiveIndex++] = kingUpLeft;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingUpLeft]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingUpLeft;
		}
		if (kingUp <= 195 && IndexInPieceVectorFromBoardIndex[kingUp] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingUp] == -1)
				MovableIndices[ActiveIndex++] = kingUp;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingUp]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingUp;
		}
		if (OneRowUpRightThreshold <= 195 && kingUpRight <= OneRowUpRightThreshold && IndexInPieceVectorFromBoardIndex[kingUpRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingUpRight] == -1)
				MovableIndices[ActiveIndex++] = kingUpRight;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingUpRight]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingUpRight;
		}
		if (kingLeft >= currentRowLeftThreshold && IndexInPieceVectorFromBoardIndex[kingLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingLeft] == -1)
				MovableIndices[ActiveIndex++] = kingLeft;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingLeft]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingLeft;
		}
		if (kingRight <= currentRowRightThreshold && IndexInPieceVectorFromBoardIndex[kingRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingRight] == -1)
				MovableIndices[ActiveIndex++] = kingRight;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingRight]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingRight;
		}
		if (OneRowDownLeftThreshold >= 0 && kingDownLeft >= OneRowDownLeftThreshold && IndexInPieceVectorFromBoardIndex[kingDownLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingDownLeft] == -1)
				MovableIndices[ActiveIndex++] = kingDownLeft;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingDownLeft]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingDownLeft;
		}
		if (kingDown >= 0 && IndexInPieceVectorFromBoardIndex[kingDown] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingDown] == -1)
				MovableIndices[ActiveIndex++] = kingDown;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingDown]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingDown;
		}
		if (kingDownRight >= 0 && kingDownRight <= OneRowDownRightThreshold && IndexInPieceVectorFromBoardIndex[kingDownRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingDownRight] == -1)
				MovableIndices[ActiveIndex++] = kingDownRight;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingDownRight]]->value * attackerPlayerSign < 0)
				MovableIndices[ActiveIndex++] = kingDownRight;
		}
	}
	else
	{ // PUSH INTO HASH KEYS
		if (kingUpLeft <= 195 && kingUpLeft >= OneRowUpLeftThreshold && IndexInPieceVectorFromBoardIndex[kingUpLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingUpLeft] == -1)
				MovableHashKeys[kingUpLeft] = kingUpLeft;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingUpLeft]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingUpLeft] = kingUpLeft;
		}
		if (kingUp <= 195 && IndexInPieceVectorFromBoardIndex[kingUp] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingUp] == -1)
				MovableHashKeys[kingUp] = kingUp;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingUp]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingUp] = kingUp;
		}
		if (OneRowUpRightThreshold <= 195 && kingUpRight <= OneRowUpRightThreshold && IndexInPieceVectorFromBoardIndex[kingUpRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingUpRight] == -1)
				MovableHashKeys[kingUpRight] = kingUpRight;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingUpRight]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingUpRight] = kingUpRight;
		}
		if (kingLeft >= currentRowLeftThreshold && IndexInPieceVectorFromBoardIndex[kingLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingLeft] == -1)
				MovableHashKeys[kingLeft] = kingLeft;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingLeft]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingLeft] = kingLeft;
		}
		if (kingRight <= currentRowRightThreshold && IndexInPieceVectorFromBoardIndex[kingRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingRight] == -1)
				MovableHashKeys[kingRight] = kingRight;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingRight]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingRight] = kingRight;
		}
		if (OneRowDownLeftThreshold >= 0 && kingDownLeft >= OneRowDownLeftThreshold && IndexInPieceVectorFromBoardIndex[kingDownLeft] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingDownLeft] == -1)
				MovableHashKeys[kingDownLeft] = kingDownLeft;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingDownLeft]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingDownLeft] = kingDownLeft;
		}
		if (kingDown >= 0 && IndexInPieceVectorFromBoardIndex[kingDown] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingDown] == -1)
				MovableHashKeys[kingDown] = kingDown;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingDown]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingDown] = kingDown;
		}
		if (kingDownRight >= 0 && kingDownRight <= OneRowDownRightThreshold && IndexInPieceVectorFromBoardIndex[kingDownRight] != -2)
		{
			if (IndexInPieceVectorFromBoardIndex[kingDownRight] == -1)
				MovableHashKeys[kingDownRight] = kingDownRight;
			else if (pieces[IndexInPieceVectorFromBoardIndex[kingDownRight]]->value * attackerPlayerSign < 0)
				MovableHashKeys[kingDownRight] = kingDownRight;
		}
	}

	// TODO: Check if the current King is qualified for Castling, if yes then highlight that as one of the movable
	// TODO: We are gonna highlight Castling differently
	//if (pieces[IndexInPieceVectorFromBoardIndex[currentIndex]]->firstMove = 1)	// is the currentKing is still valid for Castling
	//{
	//	// TODO: Check if the currentKing is being checked
	//	if (attackerPlayerSign == 1)
	//	{
	//		if (IndexInPieceVectorFromBoardIndex[originalRookIndex] <= -1) return;	// if the position of Rook is not the same then return, this is also for SafeNull
	//		if (pieces[IndexInPieceVectorFromBoardIndex[originaRookIndex]] != index-of-rook-in-PieceVector-of-team-1) return;	// if the current piece is not Team1 original Rook then return
	//		for (int i = originalRookIndex; i < originalKingIndex; i++)
	//		{
	//			if (IndexInPieceVectorFromBoardIndex[i] != -1) 
	//				return; //  not satisfactory for Castling, thus we end with what we've got
	//			/// TODO: check if the road along is being checked (any indices within the travel road is in the enemyMovables)
	//			// PSEUDO: if (IndexInPieceVectorFromBoardIndex[i] is going to be checked)
	//			// TODO: think about getting movable indices of all the enemy's pieces and see if they contains any index in the road (and maybe the king as well)
	//		}
	//		MovableIndices[ActiveIndex++] = RookIndex;
	//	}
	//	// TODO: finish with other player's turn
	//}
	//	todo: change the position of this code to makeMove(), changing the King in if (isKing()) to value 0
	// pieces[IndexInPieceVectorFromBoardIndex[currentIndex]] = 0;	// after the firstMove, we change this to 0, indicating this King is not qualified anymore

	// DEBUG purpose
	//for (int i = 0; i < ActiveIndex; i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("King MovableIndices are: %d"), MovableIndices[i]);
	//}
}

// THESE THRESHOLD SHOULD BE COMPARED TO THE THRESHOLD OF THE WHOLE BOARD, SO THAT WE ARE NOT CONFUSE
// BOTH OF THESE FUNCTIONS ARE CLAMP WITHIN [0, 195]
int16 ACustomPlayerController::LeftThresholdOfIndex(const int16& index)
{
	return std::min(std::max(FMath::Floor(index / 14) * 14, 0), 195);
}

int16 ACustomPlayerController::RightThresholdOfIndex(const int16& index)
{
	return std::min(std::max(LeftThresholdOfIndex(index) + 13, 0), 195);
}

// VALIDATION OF CLICK 1
//this might not be as good as to change to storing one hit variable and compare the second click to the variable to see if we reset the click again 
bool ACustomPlayerController::isClick1P1IndexValid(const int16& hitIndexOn196)
{
	if (IndexInPieceVectorFromBoardIndex[hitIndexOn196] < 0 || IndexInPieceVectorFromBoardIndex[hitIndexOn196] > 15) return false;
	return isIndexValid(hitIndexOn196);
}

bool ACustomPlayerController::isClick1P3IndexValid(const int16& hitIndexOn196)
{
	if (IndexInPieceVectorFromBoardIndex[hitIndexOn196] < 16 || IndexInPieceVectorFromBoardIndex[hitIndexOn196] > 31) return false;
	return isIndexValid(hitIndexOn196);
}

bool ACustomPlayerController::isClick1P2IndexValid(const int16& hitIndexOn196)
{
	if (IndexInPieceVectorFromBoardIndex[hitIndexOn196] < 32 || IndexInPieceVectorFromBoardIndex[hitIndexOn196] > 47) return false;
	return isIndexValid(hitIndexOn196);
}

bool ACustomPlayerController::isClick1P4IndexValid(const int16& hitIndexOn196)
{
	if (IndexInPieceVectorFromBoardIndex[hitIndexOn196] < 48 || IndexInPieceVectorFromBoardIndex[hitIndexOn196] > 63) return false;
	return isIndexValid(hitIndexOn196);
}

// VALIDATION OF CLICK 2
// Because after a Valid Click 1 we've already got MovableIndices of that specific player and piece -> just need 1 check for all kinds of player
bool ACustomPlayerController::isClick2MoveIndexValid(const int16& hitIndexOn196)
{
	UE_LOG(LogTemp, Warning, TEXT("hitINdexOn196: %d"), hitIndexOn196);
	UE_LOG(LogTemp, Warning, TEXT("ActiveIndex: %d"), ActiveIndex);
	// see if the MovableIndices contains the hitIndexOn196
	if (ActiveIndex == 0) return false; // if no move available, we just mark as false so that it returns to waiting from click1
	// TODO: fix: see if movable indices WITHIN ACTIVE INDEX RANGE contains the value
	for (int i = 0; i < ActiveIndex; i++)
		if (hitIndexOn196 == MovableIndices[i])
			return true;
	return false;
}

bool ACustomPlayerController::isIndexValid(const int16& index)
{
	return index > 0 && index < 196 && IndexInPieceVectorFromBoardIndex[index] != -2;
}

// Checking the type of piece
bool ACustomPlayerController::isPawn(const int16& pieceValue)
{
	return abs(pieceValue) == 1 || abs(pieceValue) == 11;
}


bool ACustomPlayerController::isRook(const int16& pieceValue)
{
	return abs(pieceValue) == 2 || abs(pieceValue) == 12;
}


bool ACustomPlayerController::isKnight(const int16& pieceValue)
{
	return abs(pieceValue) == 3 || abs(pieceValue) == 13;
}

bool ACustomPlayerController::isBishop(const int16& pieceValue)
{
	return abs(pieceValue) == 4 || abs(pieceValue) == 14;
}


bool ACustomPlayerController::isQueen(const int16& pieceValue)
{
	return abs(pieceValue) == 5 || abs(pieceValue) == 15;
}


bool ACustomPlayerController::isKing(const int16& pieceValue)
{
	return abs(pieceValue) == 6 || abs(pieceValue) == 16;
}

// TODO: we are going to set the material here instead of inside the pieces constructors
// TODO: SET THE PROPER MATERIALS FOR EACH SIDES
/* TOP */
void ACustomPlayerController::spawnBottomActors()
{
	uint8 pieceValue = 1;
	/* PAWN: 1 */
	float x = 600.f, y = 1400.f;
	for (int i = 0; i < 8; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		pieces.Top()->SetMaterialInactive(1);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		UE_LOG(LogTemp, Warning, (TEXT("indexOnboard: %d")), pieces.Top()->indexOnBoard);
		y += 400;
	}
	++pieceValue;

	/* ROOK: 2 */
	x = 200.f; y = 1400.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		// some operations on tweaking the location here
		pieces.Top()->SetMaterialInactive(1);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2800.f;
	}
	++pieceValue;

	/* KNIGHT: 3 */
	x = 200.f; y = 1800.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		pieces.Top()->SetMaterialInactive(1);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2000.f;
	}
	++pieceValue;

	/* BISHOP: 4 */
	x = 200.f; y = 2200.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		pieces.Top()->SetMaterialInactive(1);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 1200.f;
	}

	++pieceValue;

	/* QUEEN: 5 */
	x = 200.f; y = 2600.f;
	pieces.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	pieces.Top()->SetMaterialInactive(1);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));

	++pieceValue;

	/* KING: 6 */
	x = 200.f; y = 3000.f;
	pieces.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	pieces.Top()->SetMaterialInactive(1);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
}

/* LEFT */
void ACustomPlayerController::spawnLeftActors()
{
	int8 pieceValue = -1;
	/* PAWN: -1 */
	float x = 1400.f, y = 600.f;
	for (int i = 0; i < 8; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
		pieces.Top()->SetMaterialInactive(2);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 400.f;
	}

	pieceValue--;

	/* ROOK: -2 */
	x = 1400.f; y = 200.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		pieces.Top()->SetMaterialInactive(2);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2800.f;
	}
	pieceValue--;

	/* KNIGHT: -3 */
	x = 1800.f; y = 200.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
		pieces.Top()->SetMaterialInactive(2);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2000.f;
	}
	pieceValue--;

	/* BISHOP: -4 */
	x = 2200.f; y = 200.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
		pieces.Top()->SetMaterialInactive(2);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 1200.f;
	}
	pieceValue--;

	/* QUEEN */
	x = 2600.f; y = 200.f;
	pieces.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
	pieces.Top()->SetMaterialInactive(2);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = 6;

	pieceValue--;

	/* KING */
	x = 3000.f; y = 200.f;
	pieces.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	pieces.Top()->SetMaterialInactive(2);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = 7;

}

/* TOP */
void ACustomPlayerController::spawnTopActors()
{
	uint8 pieceValue = 11;
	/* PAWN: 11 */
	float x = 5000.f, y = 1400.f;
	for (int i = 0; i < 8; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
		pieces.Top()->SetMaterialInactive(3);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 400.f;
	}
	pieceValue++;

	/* ROOK: 12 */
	x = 5400.f; y = 1400.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		pieces.Top()->SetMaterialInactive(3);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2800.f;
	}
	pieceValue++;

	/* KNIGHT: 13 */
	x = 5400.f; y = 1800.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
		pieces.Top()->SetMaterialInactive(3);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2000.f;
	}
	pieceValue++;

	/* BISHOP: 14 */
	x = 5400.f; y = 2200.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
		pieces.Top()->SetMaterialInactive(3);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 1200.f;
	}

	pieceValue++;

	/* QUEEN: 15 */
	x = 5400.f; y = 3000.f;
	pieces.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
	pieces.Top()->SetMaterialInactive(3);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = 189;

	pieceValue++;

	/* KING: 16 */
	x = 5400.f; y = 2600.f;
	pieces.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	pieces.Top()->SetMaterialInactive(3);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = 188;

}

void ACustomPlayerController::spawnRightActors()
{
	int8 pieceValue = -11;
	/* PAWN */
	float x = 1400.f, y = 5000.f;
	for (int i = 0; i < 8; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
		pieces.Top()->SetMaterialInactive(4);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 400.f;
	}
	pieceValue--;

	/* ROOK */
	x = 1400.f; y = 5400.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		pieces.Top()->SetMaterialInactive(4);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2800.f;
	}
	pieceValue--;

	/* KNIGHT */
	x = 1800.f; y = 5400.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
		pieces.Top()->SetMaterialInactive(4);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2000.f;
	}
	pieceValue--;

	/* BISHOP */
	x = 2200.f; y = 5400.f;
	for (int i = 0; i < 2; i++)
	{
		pieces.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
		pieces.Top()->SetMaterialInactive(4);
		pieces.Top()->value = pieceValue;
		pieces.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 1200.f;
	}
	pieceValue--;
	/* QUEEN */
	x = 3000.f; y = 5400.f;
	pieces.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
	pieces.Top()->SetMaterialInactive(4);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = 111;

	pieceValue--;
	/* KING */
	x = 2600.f; y = 5400.f;
	pieces.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	pieces.Top()->SetMaterialInactive(4);
	pieces.Top()->value = pieceValue;
	pieces.Top()->indexOnBoard = 97;

}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Board = GetWorld()->SpawnActor<AChessBoard>();

	// same team spawn next to each other 
	spawnBottomActors();
	spawnTopActors();
	spawnLeftActors();
	spawnRightActors();

	for (int i = 0; i < pieces.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Value in array: %d. First move status: %d"), pieces[i]->value, pieces[i]->firstMove);
	}

	/*

		The guy Spawn the Board as Separate Actor (one instance of type ChessBoard)
		Then Spawn the Whites and the Blacks

		For us we can also Spawn the ChessPiece as Separate Actor (one instance of typeChessPiece)

	*/

}
