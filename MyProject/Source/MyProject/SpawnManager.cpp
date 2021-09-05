// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include <algorithm>
#include "BishopActor.h"
#include "KingActor.h"
#include "KnightActor.h"
#include "PawnActor.h"
#include "QueenActor.h"
#include "RookActor.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
// Currently non-compilable as the functions are not completed yet
// TODO: CHANGE SET MESH TO THIS? CHANGE TO ANYTHING THAT SUPPORT PLAYER PICKING SKIN
ASpawnManager::ASpawnManager()
{
	bReplicates = true;
    bAlwaysRelevant = true;
    bNetUseOwnerRelevancy = true;

	playerTurn = 1;
	clickCount = 0;
	hitIndexOnBoard_1 = -1;
	// init all the mechanics arrays and variables here
	// remember to exchange the value of the mechanic array whenever you move
	ActiveIndex = 0;
	bEnPassant = false;

	MovableIndices.Init(0, 160);
	EatableHashKeys.Init(-1, 196);

	BoardOfPieceIndexInVector = {
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

// TODO: THIS IS HAVING SOME TROUBLE RESETTING CLICKS
// TODO: begin to divide on different player turn and check if currentIndex Piece belongs to that person?
// TODO: hitValue1 is being wrong (must take PiecesVector[]->value)
void ASpawnManager::OnLeftMouseClick(FVector hitLocation)
{

	if (playerTurn == 1)	// no one gets checkMate right in turn 1 of the game thus we don't have to place a checkMate announcement here. We will just get it from the last of turn 4
	{ // the events happening in Click1
		if (clickCount == 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Left mouse click 1 is triggered!"));
			//UE_LOG(LogTemp, Warning, TEXT("current clickCount is %d"), clickCount);

			hitIndexOnBoard_1 = LocationToIndex(hitLocation);
			// TODO: make this 4 players 
			if (!isClick1P1IndexValid(hitIndexOnBoard_1)) return;

			clickCount = 1;

			const int16 hitValue1 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
			//UE_LOG(LogTemp, Warning, TEXT("current clickvalue is %d"), hitValue1);

			int16 startStorageIndex = 0;
			HighlightSelectedTileByIndex(hitIndexOnBoard_1);

			//UE_LOG(LogTemp, Warning, TEXT("Hit Value is %d!"), BoardOfPieceIndexInVector[hitIndexOnBoard_1]);
			//UE_LOG(LogTemp, Warning, TEXT("Hit Index is %d!"), hitIndexOnBoard_1);
			if (isPawn(hitValue1))
			{
				GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		// TODO: MIGHT HAVE TO CHANGE THE CONDITION CHECKING OF CLICK1 AND CLICK2 TO EXCLUDE 0 AND MAKE IT A SEPARATE CHECK
		///TODO: CHECK IF INDEX IS THE SAME, if the same then we repeat 1 + unhighlight the last selected
		// the events happening in click 2
		int16 hitIndexOnBoard_2;
		if (clickCount == 1)
		{
			hitIndexOnBoard_2 = LocationToIndex(hitLocation);
			if (hitIndexOnBoard_2 == hitIndexOnBoard_1) return;
			// THERE ARE 2 POSSIBILITIES IF hitIndexOnBoard_2 IS NOT VALID: either 1. it's NOT VALID AT ALL, 2. it's re-selection of another piece in current player's side
			if (!isClick2MoveIndexValid(hitIndexOnBoard_2))
			{
				UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is not valid at %d with value %d!"), hitIndexOnBoard_2, BoardOfPieceIndexInVector[hitIndexOnBoard_2]);
				//for (int i = 0; i < ActiveIndex; i++)
				//	UE_LOG(LogTemp, Warning, TEXT("Movable indices at %d contains index: %d!"), i, MovableIndices[i]);
				// IF IT'S THE  PLAYER CLICK AGAIN ON ONE OF HIS PIECE (we take it as re-selection) and cast it back to click1
				if (isClick1P1IndexValid(hitIndexOnBoard_2))	// re-selection happens	
				{
					// RE-ASSIGN ENPASSANT AS WE NEED TO RE-EVALUATE IT AGAIN
					bEnPassant = false;
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOnBoard_1, true);

					// AS WE RE-SELECT, WE RE-ASSIGN HITINDEX1 WITH HITINDEX2 VALUE, WAITING FOR CLICK2
					hitIndexOnBoard_1 = hitIndexOnBoard_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOnBoard_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				// TODO: we might have to add checkMate at this stage
				makeMove(hitIndexOnBoard_1, hitIndexOnBoard_2);
				// WHATEVER IT WAS, IF MOVED THEN EN PASSANT IS RESET
				bEnPassant = false;

				// we wanna check the checkMate from this specific Piece's NEW POSITION, this is check-mating from us to the enemy after we move
				// you might triggered en passant again each time you call the get movable indices, thus you have to re-assing them again each time you call to get movable indices
				checkMateToEnemyTeam(hitIndexOnBoard_2);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				//we are about to enter turn for player 2. If Player 2  are checkmating either player 1 or 3 then it's sure win
				bool bCheckMateWinForTeam2ToPlayer1 = checkMateFromPlayerXToPlayerY(-2, 1);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				bool bCheckMateWinForTeam2ToPlayer3 = checkMateFromPlayerXToPlayerY(-2, 3);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				if (bCheckMateWinForTeam2ToPlayer1 || bCheckMateWinForTeam2ToPlayer3)
					UE_LOG(LogTemp, Warning, TEXT("Team 1 lose!"));

				playerTurn = -2;
				/// announce right when turn is changed
				if (bP2Checked && !bP4Checked)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player2 is currently being checkedMate!"));
				}
				else if (bP4Checked && !bP2Checked)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player4 is currently being checkedMate!"));
				}
				else if (bP2Checked && bP4Checked)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player2 and 4 are currently being checkedMate!"));
				}
			}
		}
	}
	else if (playerTurn == -2)
	{
		// the events happening in Click1
		if (clickCount == 0)
		{
			hitIndexOnBoard_1 = LocationToIndex(hitLocation);
			if (!isClick1P2IndexValid(hitIndexOnBoard_1)) return;

			clickCount = 1;
			const int16 hitValue1 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
			int16 startStorageIndex = 0;
			HighlightSelectedTileByIndex(hitIndexOnBoard_1);

			if (isPawn(hitValue1))
			{
				GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		int16 hitIndexOnBoard_2;
		if (clickCount == 1)
		{
			hitIndexOnBoard_2 = LocationToIndex(hitLocation);
			if (hitIndexOnBoard_2 == hitIndexOnBoard_1) return;

			if (!isClick2MoveIndexValid(hitIndexOnBoard_2))
			{
				// RE-ASSIGN ENPASSANT AS WE NEED TO RE-EVALUATE IT AGAIN
				// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
				if (isClick1P2IndexValid(hitIndexOnBoard_2))
				{
					bEnPassant = false;
					//DEBUG
					//UE_LOG(LogTemp, Warning, TEXT("Cast back to click1 from click2"));
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOnBoard_1, true);
					// just assign hitIndex1 <- hitIndex2, as we re-select and see this as click1 again, waiting for click2
					hitIndexOnBoard_1 = hitIndexOnBoard_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOnBoard_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				makeMove(hitIndexOnBoard_1, hitIndexOnBoard_2);
				bEnPassant = false;

				checkMateToEnemyTeam(hitIndexOnBoard_2);
				EatableHashKeys.Init(-1, 196);
				// you might triggered en passant again each time you call the get movable indices, thus you have to re-assing them again
				bEnPassant = false;

				//we are about to enter turn for player 3. If Player 3 are checkmating either player 2 or 4 then it's sure win
				bool bCheckMateWinForTeam1ToPlayer2 = checkMateFromPlayerXToPlayerY(3, -2);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				bool bCheckMateWinForTeam1ToPlayer4 = checkMateFromPlayerXToPlayerY(3, -4);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				if (bCheckMateWinForTeam1ToPlayer2 || bCheckMateWinForTeam1ToPlayer4)
					UE_LOG(LogTemp, Warning, TEXT("Team 2 lose!"));

				playerTurn = 3;

				// check after the turn is changed
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
			}
		}
	}
	else if (playerTurn == 3)
	{
		// the events happening in Click1
		if (clickCount == 0)
		{
			hitIndexOnBoard_1 = LocationToIndex(hitLocation);
			if (!isClick1P3IndexValid(hitIndexOnBoard_1)) return;

			clickCount = 1;
			const int16 hitValue1 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
			int16 startStorageIndex = 0;
			HighlightSelectedTileByIndex(hitIndexOnBoard_1);

			if (isPawn(hitValue1))
			{
				GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		int16 hitIndexOnBoard_2;
		if (clickCount == 1)
		{
			hitIndexOnBoard_2 = LocationToIndex(hitLocation);
			if (hitIndexOnBoard_2 == hitIndexOnBoard_1) return;
			if (!isClick2MoveIndexValid(hitIndexOnBoard_2))
			{
				// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
				if (isClick1P3IndexValid(hitIndexOnBoard_2))
				{   // RE-ASSIGN ENPASSANT AS WE NEED TO RE-EVALUATE IT AGAIN
					bEnPassant = false;
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOnBoard_1, true);

					hitIndexOnBoard_1 = hitIndexOnBoard_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOnBoard_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				makeMove(hitIndexOnBoard_1, hitIndexOnBoard_2);
				bEnPassant = false;

				// KhanhCD Modify Start 2021-08-22
				checkMateToEnemyTeam(hitIndexOnBoard_2);
				EatableHashKeys.Init(-1, 196);
				// you might triggered en passant again each time you call the get movable indices, thus you have to re-assing them again
				bEnPassant = false;

				//we are about to enter turn for player 2. If Player 2  are checkmating either player 1 or 3 then it's sure win
				bool bCheckMateWinForTeam2ToPlayer1 = checkMateFromPlayerXToPlayerY(-4, 1);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				bool bCheckMateWinForTeam2ToPlayer3 = checkMateFromPlayerXToPlayerY(-4, 3);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				if (bCheckMateWinForTeam2ToPlayer1 || bCheckMateWinForTeam2ToPlayer3)
					UE_LOG(LogTemp, Warning, TEXT("Team 1 lose!"));

				playerTurn = -4;
				/// check instantly when the turn changed to player 4
				if (bP2Checked && !bP4Checked)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player2 is currently being checkedMate!"));
				}
				else if (bP4Checked && !bP2Checked)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player4 is currently being checkedMate!"));
				}
				else if (bP2Checked && bP4Checked)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player2 and 4 are currently being checkedMate!"));
				}
			}
		}
	}
	else if (playerTurn == -4)
	{
		// the events happening in Click1
		if (clickCount == 0)
		{
			hitIndexOnBoard_1 = LocationToIndex(hitLocation);
			if (!isClick1P4IndexValid(hitIndexOnBoard_1)) return;

			clickCount = 1;
			const int16 hitValue1 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
			int16 startStorageIndex = 0;

			HighlightSelectedTileByIndex(hitIndexOnBoard_1);
			if (isPawn(hitValue1))
			{
				GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isRook(hitValue1))
			{
				GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKnight(hitValue1))
			{
				GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isBishop(hitValue1))
			{
				GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isQueen(hitValue1))
			{
				GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			else if (isKing(hitValue1))
			{
				GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
				HighlightMovableIndices(0);
			}
			return;
		}

		int16 hitIndexOnBoard_2;
		if (clickCount == 1)
		{
			hitIndexOnBoard_2 = LocationToIndex(hitLocation);
			if (hitIndexOnBoard_2 == hitIndexOnBoard_1) return;
			if (!isClick2MoveIndexValid(hitIndexOnBoard_2))
			{
				// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
				if (isClick1P4IndexValid(hitIndexOnBoard_2))
				{
					bEnPassant = false;
					// unhighlight all the past highlighted tiles
					for (int i = 0; i < ActiveIndex; i++)
						UnhighlightTileByIndex(MovableIndices[i], false);
					UnhighlightTileByIndex(hitIndexOnBoard_1, true);
					// just assign hitIndex1 <- hitIndex2, as we re-select and see this as click1 again, waiting for click2
					hitIndexOnBoard_1 = hitIndexOnBoard_2;

					// recalculate and re-highlight all the stuffs just like click1
					const int16 hitValue2 = PiecesVector[BoardOfPieceIndexInVector[hitIndexOnBoard_1]]->value;
					HighlightSelectedTileByIndex(hitIndexOnBoard_1);
					int16 startStorageIndex = 0;

					if (isPawn(hitValue2))
					{
						GetMovableIndicesPawn(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isRook(hitValue2))
					{
						GetMovableIndicesRook(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKnight(hitValue2))
					{
						GetMovableIndicesKnight(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isBishop(hitValue2))
					{
						GetMovableIndicesBishop(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isQueen(hitValue2))
					{
						GetMovableIndicesQueen(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
					else if (isKing(hitValue2))
					{
						GetMovableIndicesKing(startStorageIndex, hitIndexOnBoard_1, false, playerTurn);
						HighlightMovableIndices(0);
					}
				}
			}
			else
			{
				clickCount = 0;
				makeMove(hitIndexOnBoard_1, hitIndexOnBoard_2);
				bEnPassant = false;

				checkMateToEnemyTeam(hitIndexOnBoard_2);
				EatableHashKeys.Init(-1, 196);
				// you might triggered en passant again each time you call the get movable indices, thus you have to re-assing them again
				bEnPassant = false;

				//we are about to enter turn for player 3. If Player 3 are checkmating either player 2 or 4 then it's sure win
				bool bCheckMateWinForTeam1ToPlayer2 = checkMateFromPlayerXToPlayerY(1, -2);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				bool bCheckMateWinForTeam1ToPlayer4 = checkMateFromPlayerXToPlayerY(1, -4);
				EatableHashKeys.Init(-1, 196);
				bEnPassant = false;

				if (bCheckMateWinForTeam1ToPlayer2 || bCheckMateWinForTeam1ToPlayer4)
					UE_LOG(LogTemp, Warning, TEXT("Team 2 lose!"));

				playerTurn = 1;
				/// check instantly when the turn changed to player 1
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
			}
		}
	}
}

// MUST GET THE CURRENT TRANSFORM OF PIECE SO THAT WE CAN ALSO COPY THE ROTATION, as pieces convert back to their Instanced Original Rotation as soon as we don't plug in current Rotator 
// TODO: IF THE PLACE TO GO TO == 0 BUT IS DOING EN PASSANT
// TODO: IF THE PLACE TO GO TO != 0 (BELONGS TO THE ENEMY)
// TODO: add the checkMate at the end of the turn 
//  TODO: check both from enemy to us and from us to the enemy
void ASpawnManager::makeMove(const int16& boardIndex_1, int16 boardIndex_2)
{
	const int16 IndexInPieceVector = BoardOfPieceIndexInVector[boardIndex_1];
	const int16 pieceValue = PiecesVector[IndexInPieceVector]->value;

	bool bMoveCastling = false;

	if (isPawn(pieceValue) || isKing(pieceValue) || isRook(pieceValue))
		PiecesVector[IndexInPieceVector]->moveCount += 1;
	if (isPawn(pieceValue) && PiecesVector[IndexInPieceVector]->moveCount == 9)
	{
		// pseudo code for transformation:
		// PiecesVector[IndexInPieceVector]->value = value_of_piece_type_you_want_to_transform_to;
		// because we get movables and stuffs based on pieceValue, just by reassigning the value in pieceVector, we have successfully transformed the piece
		// now all we have to do is change the Actor Mesh as well!
		// TODO: add changeMesh() method to parentActor
	}

	FVector newLocation = BoardIndexToLocation(boardIndex_2);
	PiecesVector[IndexInPieceVector]->SetActorLocation(newLocation);

	// IF THE MOVE IS TO EAT
	if (BoardOfPieceIndexInVector[boardIndex_2] != -1)
	{	// NORMAL EATING CASE
		moveToGraveyard(boardIndex_2);
		// update the indexOnboard in the pieceVector
		PiecesVector[BoardOfPieceIndexInVector[boardIndex_2]]->indexOnBoard = -1;
		// then update on the chess board for later swapping
		BoardOfPieceIndexInVector[boardIndex_2] = -1;

		// unhighlight all the suggested Tiles
		for (int i = 0; i < ActiveIndex; i++)
			UnhighlightTileByIndex(MovableIndices[i], false);
		UnhighlightTileByIndex(boardIndex_1, true);

		// in the pieceVector, we update the indexOnBoard of the vector to new location
		PiecesVector[BoardOfPieceIndexInVector[boardIndex_1]]->indexOnBoard = boardIndex_2;
		// then we swap the index. We already set the newLocation = -1 (and for EnPassant the eaten place = -1), so we can swap now
		BoardOfPieceIndexInVector.Swap(boardIndex_1, boardIndex_2);
	}
	else
	{	
		if (isPawn(pieceValue))
		{
			// ENPASSANT EATING CASE: from analysis, en passant position can only fall within the 'cross' (4 indices) around index2
			if (bEnPassant)
			{	// TODO: divide this into playerTurn (so that it not eat the one matching the condition first)
				int16 indexToGraveyard;
				if (playerTurn == -2 || playerTurn == -4)
				{ // for team 2 (left and right) only check top and bottom
					if (BoardOfPieceIndexInVector[boardIndex_2 + 14] > -1)	// '> -1' condition hedge against '-2' as well so it's better
						indexToGraveyard = boardIndex_2 + 14;
					else if (BoardOfPieceIndexInVector[boardIndex_2 - 14] > -1)
						indexToGraveyard = boardIndex_2 - 14;
				}
				if (playerTurn == 1 || playerTurn == 3)
				{ // for team 1 (top and bottom) only check left and right
					if (BoardOfPieceIndexInVector[boardIndex_2 - 1] > -1)
						indexToGraveyard = boardIndex_2 - 1;
					if (BoardOfPieceIndexInVector[boardIndex_2 + 1] > -1)
						indexToGraveyard = boardIndex_2 + 1;
				}
				moveToGraveyard(indexToGraveyard);
				// update the index of the eaten piece to =-1
				PiecesVector[BoardOfPieceIndexInVector[indexToGraveyard]]->indexOnBoard = -1;
				// then update index on the chess board for correct swapping values later
				BoardOfPieceIndexInVector[indexToGraveyard] = -1;
			}
			// reset EnPassant to false again
			bEnPassant = false;

		}
		else if (isKing(pieceValue))
		{ // TODO: DO FOR ALL 4 TEAMS CASTLING
			if (bP1Castling) 
			{ 
				if (boardIndex_2 == 9)  // right castling - short
				{ //  king has already been set to new location because King is index196_1 - the index that's fits normal location setting
					// we only have to exchange the Rook piece
					PiecesVector[9]->SetActorLocation(BoardIndexToLocation(8));
					// update Board index of king and Rook pieces in piece Vector 
					PiecesVector[15]->indexOnBoard = 9;
					PiecesVector[9]->indexOnBoard = 8;
					// new Board indices that holds the piece index
					BoardOfPieceIndexInVector[8] = 9;
					BoardOfPieceIndexInVector[9] = 15;
					// set the old board indices to -1 indicating empty
					BoardOfPieceIndexInVector[7] = -1;
					BoardOfPieceIndexInVector[10] = -1;
					bMoveCastling = true;
				}
				else if (boardIndex_2 == 5) // left castling - long
				{
					PiecesVector[8]->SetActorLocation(BoardIndexToLocation(6));
					PiecesVector[15]->indexOnBoard = 5;
					PiecesVector[8]->indexOnBoard = 6;
					BoardOfPieceIndexInVector[6] = 8;
					BoardOfPieceIndexInVector[5] = 15;
					BoardOfPieceIndexInVector[7] = -1;
					BoardOfPieceIndexInVector[3] = -1;
					bMoveCastling = true;
				}
				// assign to false so that we don't mess up with other team Castling
				bP1Castling = false;
			}
			else if (bP2Castling) 
			{ 
				if (boardIndex_2 == 126)  // top castling - short 
				{ //  king has already been set to new location because King is index196_1 - the index that's fits normal location setting
					// we only have to exchange the Rook piece
					PiecesVector[41]->SetActorLocation(BoardIndexToLocation(112));
					// update Board index of king and Rook pieces in piece Vector 
					PiecesVector[47]->indexOnBoard = 126;
					PiecesVector[41]->indexOnBoard = 112;
					// new Board indices that holds the piece index
					BoardOfPieceIndexInVector[112] = 41;
					BoardOfPieceIndexInVector[126] = 47;
					// set the old board indices to -1 indicating empty
					BoardOfPieceIndexInVector[140] = -1; 
					BoardOfPieceIndexInVector[98] = -1;
					bMoveCastling = true;
				}
				else if (boardIndex_2 == 70) // bottom castling - long
				{
					PiecesVector[40]->SetActorLocation(BoardIndexToLocation(84));
					PiecesVector[47]->indexOnBoard = 70;
					PiecesVector[40]->indexOnBoard = 84;
					BoardOfPieceIndexInVector[70] = 47;
					BoardOfPieceIndexInVector[84] = 40;
					BoardOfPieceIndexInVector[98] = -1;
					BoardOfPieceIndexInVector[42] = -1;
					bMoveCastling = true;
				}
				// assign to false so that we don't mess up with other team Castling
				bP2Castling = false;
			}
			else if (bP3Castling) 
			{ 
				if (boardIndex_2 == 186)  // left castling - short 
				{ //  king has already been set to new location because King is index196_1 - the index that's fits normal location setting
					// we only have to exchange the Rook piece
					PiecesVector[24]->SetActorLocation(BoardIndexToLocation(187));
					// update Board index of king and Rook pieces in piece Vector 
					PiecesVector[31]->indexOnBoard = 186;
					PiecesVector[24]->indexOnBoard = 187;
					// new Board indices that holds the piece index
					BoardOfPieceIndexInVector[186] = 31;
					BoardOfPieceIndexInVector[187] = 24;
					// set the old board indices to -1 indicating empty
					BoardOfPieceIndexInVector[188] = -1; 
					BoardOfPieceIndexInVector[185] = -1;
					bMoveCastling = true;
				}
				else if (boardIndex_2 == 190) // right castling - long
				{
					PiecesVector[25]->SetActorLocation(BoardIndexToLocation(189));
					PiecesVector[31]->indexOnBoard = 190;
					PiecesVector[25]->indexOnBoard = 189;
					BoardOfPieceIndexInVector[190] = 31;
					BoardOfPieceIndexInVector[189] = 25;
					BoardOfPieceIndexInVector[188] = -1;
					BoardOfPieceIndexInVector[192] = -1;
					bMoveCastling = true;
				}
				// assign to false so that we don't mess up with other team Castling
				bP3Castling = false;
			}
			else if (bP4Castling) 
			{ 
				if (boardIndex_2 == 69)  // bottom castling - short 
				{ //  king has already been set to new location because King is index196_1 - the index that's fits normal location setting
					// we only have to exchange the Rook piece
					PiecesVector[56]->SetActorLocation(BoardIndexToLocation(83));
					// update Board index of king and Rook pieces in piece Vector 
					PiecesVector[63]->indexOnBoard = 69;
					PiecesVector[56]->indexOnBoard = 83;
					// new Board indices that holds the piece index
					BoardOfPieceIndexInVector[69] = 63;
					BoardOfPieceIndexInVector[83] = 56;
					// set the old board indices to -1 indicating empty
					BoardOfPieceIndexInVector[97] = -1; 
					BoardOfPieceIndexInVector[55] = -1;
					bMoveCastling = true;
				}
				else if (boardIndex_2 == 125) // top castling - long
				{
					PiecesVector[57]->SetActorLocation(BoardIndexToLocation(111));
					PiecesVector[63]->indexOnBoard = 125;
					PiecesVector[57]->indexOnBoard = 111;
					BoardOfPieceIndexInVector[125] = 63;
					BoardOfPieceIndexInVector[111] = 57;
					BoardOfPieceIndexInVector[97] = -1;
					BoardOfPieceIndexInVector[153] = -1;
					bMoveCastling = true;
				}
				// assign to false so that we don't mess up with other team Castling
				bP4Castling = false;
			}
		}

		if (bMoveCastling)
		{
			// unhighlight all the suggested Tiles
			for (int i = 0; i < ActiveIndex; i++)
				UnhighlightTileByIndex(MovableIndices[i], false);
			UnhighlightTileByIndex(boardIndex_1, true);
		}
		else
		{
			// unhighlight all the suggested Tiles
			for (int i = 0; i < ActiveIndex; i++)
				UnhighlightTileByIndex(MovableIndices[i], false);
			UnhighlightTileByIndex(boardIndex_1, true);

			// in the pieceVector, we update the indexOnBoard of the vector to new location
			PiecesVector[BoardOfPieceIndexInVector[boardIndex_1]]->indexOnBoard = boardIndex_2;
			// then we swap the index. We already set the newLocation = -1 (and for EnPassant the eaten place = -1), so we can swap now
			BoardOfPieceIndexInVector.Swap(boardIndex_1, boardIndex_2);
		}
	}
}

// TODO: THIS IS CURRENT WRONG FOR EN PASSANT AS WE HAVE ALREADY PASSED THE -1 TILE, THE ENEMY TILE HAS NOT BEEN ASSIGNED TO EMPTY 
void ASpawnManager::moveToGraveyard(const int16& hitIndexOnBoard_2)
{ // TODO: MOVE the type to appropriate place (now it's all move to same place)

	const int8 IndexInPieceVector = BoardOfPieceIndexInVector[hitIndexOnBoard_2];
	//UE_LOG(LogTemp, Warning, TEXT("IndexInPieceVector: %d"), IndexInPieceVector);
	//UE_LOG(LogTemp, Warning, TEXT("hitIndexOnBoard_2: %d"), hitIndexOnBoard_2);
	const int8 pieceValue = PiecesVector[IndexInPieceVector]->value;

	// TODO: there will be separate places for each type of piece, not currently implemented
	if (isPawn(pieceValue))
		PiecesVector[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isRook(pieceValue))
		PiecesVector[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isKnight(pieceValue))
		PiecesVector[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isBishop(pieceValue))
		PiecesVector[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isQueen(pieceValue))
		PiecesVector[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
	if (isKing(pieceValue))
		PiecesVector[IndexInPieceVector]->SetActorLocation(FVector::ZeroVector);
}

/* EXTRA FEATURES */
// TODO: CREATE INSTANCED MESH COMPONENTS, OR SOMETHING THAT WE CAN CHANGE FROM x3 to x4 etc..
// we can place before hand with visibility = 0 then once there's a shit eaten we can ..

// We will have materials at FIXED indices that serve the following function:
// [0]. Coloring Tile type1 (depends on Tile)
// [1]. Coloring Tile type2 (depends on Tile)
// [2]. Pure highlight (default = 0.f)
// TODO: create the highlight color on another Lerp so that we can toggle on or off on a specific channel using specific DataValue
void ASpawnManager::HighlightSelectedTileByLocation(const FVector& hitLocation)
{
	//if (!Board)
	//	UE_LOG(LogTemp, Warning, TEXT("The board is NUL"));
	const FVector2D XYIndex = LocationToXY(hitLocation);
	const int16 indexToHighlight = XYToIndex(XYIndex.X, XYIndex.Y);
	//UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

void ASpawnManager::HighlightSelectedTileByIndex(const int16& indexToHighlight)
{
	//if (!Board)
	//	UE_LOG(LogTemp, Warning, TEXT("The board is NULL"));
	//UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

// TODO: THIS IS NOT FINISHED, the color numeric values are just place holders
// Board is still ISM, thus we still keep this code
void ASpawnManager::HighlightMovableIndices(const int16& startMovableIndex = 0)	// by default we pass 0 if no value is passed in
{
	if (ActiveIndex != 0)
	{
		// highlight all movable indices
		int16 i = startMovableIndex;
		int16 index196;
		//UE_LOG(LogTemp, Warning, TEXT("Active index at HMI: %d"), ActiveIndex);
		for (/*  */; i < ActiveIndex - 1; i++)
		{
			index196 = MovableIndices[i];
			if (BoardOfPieceIndexInVector[index196] == -1)
				// Normal Highlight (blank tile)
				Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, false);
			else
			{
				if (bEnPassant)
				{   // highlight RED at enPassant eatable place
					if (BoardOfPieceIndexInVector[index196 + 14] > -1)
						Board->InstancedStaticMeshComponent->SetCustomDataValue(index196 + 14, 0, 1.f, false);
					else if (BoardOfPieceIndexInVector[index196 - 14] > -1)
						Board->InstancedStaticMeshComponent->SetCustomDataValue(index196 - 14, 0, 1.f, false);
					else if (BoardOfPieceIndexInVector[index196 - 1] > -1)
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
		//UE_LOG(LogTemp, Warning, TEXT("last movable Index is: %d"), index196);
		// same check logic
		if (BoardOfPieceIndexInVector[index196] == -1)
			Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, true);
		else
			Board->InstancedStaticMeshComponent->SetCustomDataValue(index196, 0, 1.f, true);
	}
}

void ASpawnManager::UnhighlightTileByIndex(const int16& index, const bool& bMarkRenderStateDirty = false)
{// change board material back to its original state
 // TODO: do a correct implementation of this after change the M_material in Unreal Editor
	Board->InstancedStaticMeshComponent->SetCustomDataValue(index, 0, 0.f, bMarkRenderStateDirty);
}

FVector2D ASpawnManager::LocationToXY(const FVector& hitLocation)
{
	//UE_LOG(LogTemp, Warning, TEXT("Location2Index: Receive Location: %s"), *hitLocation.ToString());
	return FVector2D(hitLocation.X, hitLocation.Y);
}

int16 ASpawnManager::XYToIndex(const int16& XValue, const int16& YValue)
{
	return XValue / 400 * 14 + YValue / 400;
}

FVector ASpawnManager::BoardIndexToLocation(const int16& index)
{
	int16 X = index / 14 * 400 + 200;
	int16 Y = index % 14 * 400 + 200;
	return FVector(X, Y, 0.f);
}

// TODO: CREATE UE_LOG FOR ANY TEAM THAT'S CHECKMATED (AND CHECKMATING OTHER TEAM? maybe?) SUCCEED FOR THE PURPOSE OF DEBUGGING
// as we only call this check after successfully makeMove() to index196_2, each getMovableIndices() function is sure to be valid (as makeMove() only successfully executed if the 2nd (or move-to) index is valid)
void ASpawnManager::checkMateToEnemyTeam(const int16& boardIndex)
{
	const int16 kingBottomPosition = PiecesVector[15]->indexOnBoard;
	const int16 kingTopPosition = PiecesVector[31]->indexOnBoard;
	const int16 kingLeftPosition = PiecesVector[47]->indexOnBoard;
	const int16 kingRightPosition = PiecesVector[63]->indexOnBoard;

	const int16 indexInPieceVector = BoardOfPieceIndexInVector[boardIndex];

	/// because this check mate check from our team to the enemy team, our team is the attacker, thus we pass the 'playerTurn' as the attacker sign
	if (isPawn(PiecesVector[indexInPieceVector]->value))
		GetMovableIndicesPawn(ActiveIndex, boardIndex, true, playerTurn);
	else if (isRook(PiecesVector[indexInPieceVector]->value))
		GetMovableIndicesRook(ActiveIndex, boardIndex, true, playerTurn);
	else if (isKnight(PiecesVector[indexInPieceVector]->value))
		GetMovableIndicesKnight(ActiveIndex, boardIndex, true, playerTurn);
	else if (isBishop(PiecesVector[indexInPieceVector]->value))
		GetMovableIndicesBishop(ActiveIndex, boardIndex, true, playerTurn);
	else if (isKing(PiecesVector[indexInPieceVector]->value))
		GetMovableIndicesKing(ActiveIndex, boardIndex, true, playerTurn);
	else if (isQueen(PiecesVector[indexInPieceVector]->value))
		GetMovableIndicesQueen(ActiveIndex, boardIndex, true, playerTurn);

	if (playerTurn == 1 || playerTurn == 3)
	{
		if (EatableHashKeys[kingLeftPosition] == kingLeftPosition)
			bP2Checked = true;
		if (EatableHashKeys[kingRightPosition] == kingRightPosition)
			bP4Checked = true;
	}
	else if (playerTurn == -2 || playerTurn == -4)
	{
		if (EatableHashKeys[kingBottomPosition] == kingBottomPosition)
			bP1Checked = true;
		if (EatableHashKeys[kingTopPosition] == kingTopPosition)
			bP3Checked = true;
	}
}

bool ASpawnManager::checkMateFromPlayerXToPlayerY(const int16& playerX, const int16& playerY)
{
	if (playerX == 1)
	{
		/* TEAM 1 */
		// The attackPlayerSign is for used in any eating move that uses '* attackPlayerSign' to check the validity of the eat index 
		for (int i = 0; i <= 7; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		for (int i = 8; i <= 9; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		for (int i = 10; i <= 11; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		for (int i = 12; i <= 13; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		if (PiecesVector[14]->indexOnBoard != -1)
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[14]->indexOnBoard, true, 1);
		if (PiecesVector[15]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[15]->indexOnBoard, true, 1);
		if (playerY == -2)
		{
			const int P2KingPosition = PiecesVector[47]->indexOnBoard;
			if (EatableHashKeys[P2KingPosition] == P2KingPosition) 
				return true;
		}
		else if (playerY == -4)
		{
			const int P4KingPosition = PiecesVector[63]->indexOnBoard;
			if (EatableHashKeys[P4KingPosition] == P4KingPosition) 
				return true;
		}
	}
	else if (playerX == -2)
	{
		for (int i = 32; i <= 39; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		for (int i = 40; i <= 41; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		for (int i = 42; i <= 43; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		for (int i = 44; i <= 45; i++)
		{ // getting indices from bishop is obtaining wrong indices (not getting blocked or something?) that it takes in even the indices of king and queen
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		if (PiecesVector[46]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[46]->indexOnBoard, true, -2);
		if (PiecesVector[47]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[47]->indexOnBoard, true, -2);
		if (playerY == 1)
		{
			const int P1KingPosition = PiecesVector[15]->indexOnBoard;
			if (EatableHashKeys[P1KingPosition] == P1KingPosition)
				return true;
		}
		else if (playerY == 3)
		{
			const int P3KingPosition = PiecesVector[31]->indexOnBoard;
			if (EatableHashKeys[P3KingPosition] == P3KingPosition)
				return true;
		}
	}
	else if (playerX == 3)
	{
		for (int i = 16; i <= 23; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		for (int i = 24; i <= 25; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		for (int i = 26; i <= 27; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		for (int i = 28; i <= 29; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		if (PiecesVector[30]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[30]->indexOnBoard, true, 3);
		if (PiecesVector[31]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[31]->indexOnBoard, true, 3);
		if (playerY == -2)
		{
			const int P2KingPosition = PiecesVector[47]->indexOnBoard;
			if (EatableHashKeys[P2KingPosition] == P2KingPosition)
				return true;
		}
		else if (playerY == -4)
		{
			const int P4KingPosition = PiecesVector[63]->indexOnBoard;
			if (EatableHashKeys[P4KingPosition] == P4KingPosition)
				return true;
		}
	}
	else if (playerX == -4)
	{
		for (int i = 48; i <= 55; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		for (int i = 56; i <= 57; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		for (int i = 58; i <= 59; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		for (int i = 60; i <= 61; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		if (PiecesVector[62]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[62]->indexOnBoard, true, -4);
		if (PiecesVector[63]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[63]->indexOnBoard, true, -4);
		if (playerY == 1)
		{
			const int P1KingPosition = PiecesVector[15]->indexOnBoard;
			if (EatableHashKeys[P1KingPosition] == P1KingPosition)
				return true;
		}
		else if (playerY == 3)
		{
			const int P3KingPosition = PiecesVector[31]->indexOnBoard;
			if (EatableHashKeys[P3KingPosition] == P3KingPosition)
				return true;
		}
	}
	return false;
}

// TODO: THIS IS JUST USED FOR CASTLING
void ASpawnManager::checkMateToWholeTeamOfPlayerNumber(const int16& playerNumber)
{
	if (playerNumber == 1 || playerNumber == 3)
	{   // get all movables of team 2 & team 4 into hashKeys
		const int P1KingPosition = PiecesVector[15]->indexOnBoard;
		const int P3KingPosition = PiecesVector[31]->indexOnBoard;
		/* TEAM 2 */
		// The attackPlayerSign is for used in any eating move that uses '* attackPlayerSign' to check the validity of the eat index 
		for (int i = 32; i <= 39; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		for (int i = 40; i <= 41; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		for (int i = 42; i <= 43; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		for (int i = 44; i <= 45; i++)
		{ // getting indices from bishop is obtaining wrong indices (not getting blocked or something?) that it takes in even the indices of king and queen
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -2);
		}
		if (PiecesVector[46]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[46]->indexOnBoard, true, -2);
		if (PiecesVector[47]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[47]->indexOnBoard, true, -2);

		/* TEAM 4 */
		for (int i = 48; i <= 55; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		for (int i = 56; i <= 57; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		for (int i = 58; i <= 59; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		for (int i = 60; i <= 61; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, -4);
		}
		if (PiecesVector[62]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[62]->indexOnBoard, true, -4);
		if (PiecesVector[63]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[63]->indexOnBoard, true, -4);
		if (EatableHashKeys[P1KingPosition] == P1KingPosition) 
			bP1Checked = true;
		else 
			bP1Checked = false;
		// SAME LOGIC APPLIES HERE
		if (EatableHashKeys[P3KingPosition] == P3KingPosition) 
			bP3Checked = true;
		else 
			bP3Checked = false;
	}
	else if (playerNumber == -2 || playerNumber == -4)
	{   // get all movables of team 1 & team 3 into hashKeys
		const int P2KingPosition = PiecesVector[47]->indexOnBoard;
		const int P4KingPosition = PiecesVector[63]->indexOnBoard;
		/* TEAM 1 */
		// The attackPlayerSign is for used in any eating move that uses '* attackPlayerSign' to check the validity of the eat index 
		for (int i = 0; i <= 7; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		for (int i = 8; i <= 9; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		for (int i = 10; i <= 11; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		for (int i = 12; i <= 13; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 1);
		}
		if (PiecesVector[14]->indexOnBoard != -1)
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[14]->indexOnBoard, true, 1);
		if (PiecesVector[15]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[15]->indexOnBoard, true, 1);

		/* TEAM 3 */
		for (int i = 16; i <= 23; i++)
		{ // getting wrong movables on this - getting 97 (indice of player 4 King?)
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesPawn(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		for (int i = 24; i <= 25; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesRook(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		for (int i = 26; i <= 27; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesKnight(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		for (int i = 28; i <= 29; i++)
		{
			if (PiecesVector[i]->indexOnBoard != -1) 
				GetMovableIndicesBishop(ActiveIndex, PiecesVector[i]->indexOnBoard, true, 3);
		}
		if (PiecesVector[30]->indexOnBoard != -1) 
			GetMovableIndicesQueen(ActiveIndex, PiecesVector[30]->indexOnBoard, true, 3);
		if (PiecesVector[31]->indexOnBoard != -1) 
			GetMovableIndicesKing(ActiveIndex, PiecesVector[31]->indexOnBoard, true, 3);
		if (EatableHashKeys[P2KingPosition] == P2KingPosition)
			bP2Checked = true;
		else
			bP2Checked = false;
		// SAME LOGIC APPLIES HERE
		if (EatableHashKeys[P4KingPosition] == P4KingPosition)
			bP4Checked = true;
		else
			bP4Checked = false;
	}
}

int16 ASpawnManager::LocationToIndex(const FVector& hitLocation)
{
	return FMath::Floor(hitLocation.X / 400) * 14 + FMath::Floor(hitLocation.Y / 400);
}
/* TODO: Lam function NhapThanh() */
// TODO: NOW, CASTLING SHOULD BE CONSIDERED AS A MOVABLES OF THE KING, UNTIL THE KING IS NO LONGER APPROPRIATE FOR THAT MOVE

// TODO: MAKE ENPASSANT HAPPENS ONLY AT 4 CORNERS OF THE TABLE
void ASpawnManager::GetMovableIndicesPawn(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerTurn)
{ // TODO: Segfault error
	//UE_LOG(LogTemp, Warning, TEXT("GetMovablesPawn is triggered!"));
	ActiveIndex = startStorageIndex;
	bEnPassant = false;

	// TODO: for each team the Pawn will be moving in different axes, make sure to do them after you've done networking
	// for bottom team - POSITIVE
	if (attackerTurn == 1)
	{
		/* MOVE TO MOVE MOVE: STRAIGHT FORWARD */
		if (currentIndex + 14 < 196)
		{
			if (BoardOfPieceIndexInVector[currentIndex + 14] != -2)
			{
				if (BoardOfPieceIndexInVector[currentIndex + 14] == -1)
				{
					//UE_LOG(LogTemp, Warning, TEXT("1 move ahead is fired!"));
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex + 14;
				}
				// if tile in front is a Pawn
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 14]]->value == -1)
				{ // if its move count = 1
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 14]]->moveCount == 1)
					{ // if its just straight up from Rook
						if (currentIndex + 14 == 45 || currentIndex + 14 == 143)
						{ // then en passant
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex + 13;
							bEnPassant = true;
						}
					}
				}
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 14]]->value == -11)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 14]]->moveCount == 1)
					{
						if (currentIndex + 14 == 150 || currentIndex + 14 == 52)
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex + 15;
						bEnPassant = true;
					}
				}
			}
		}

		/*  THE MOVE TO EAT */
		int16 eatRight = currentIndex + 15;
		int16 eatLeft = currentIndex + 13;
		int16 rightThreshold = RightThresholdOfIndex(eatRight);
		int16 leftThreshold = LeftThresholdOfIndex(eatRight);

		if (eatRight <= rightThreshold)
		{
			// we are seeing the move the eat so the value should not be -1 or -2, which means it's >= 0
			if (BoardOfPieceIndexInVector[eatRight] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatRight]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatRight;
					else
						EatableHashKeys[eatRight] = eatRight;
				}
			}
		}
		if (eatLeft >= leftThreshold)
		{
			if (BoardOfPieceIndexInVector[eatLeft] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatLeft]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatLeft;
					else
						EatableHashKeys[eatLeft] = eatLeft;
				}
			}
		}
	}
	else if (attackerTurn == -2)
	{ // for left team - NEGATIVE
		const int16 moveRightThreshold = RightThresholdOfIndex(currentIndex);
		/* THE MOVE TO MOVE - STRAIGHT RIGHT */
		if (currentIndex + 1 < moveRightThreshold)
		{
			if (BoardOfPieceIndexInVector[currentIndex + 1] != -2)
			{
				if (BoardOfPieceIndexInVector[currentIndex + 1] == -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex + 1;
				}
				// EN PASSANT:
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 1]]->value == 1)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 1]]->moveCount == 1)
					{
						if (currentIndex + 1 == 45 || currentIndex + 1 == 52)
						{
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex - 13;
							bEnPassant = true;
						}
					}
				}
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 1]]->value == 11)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex + 1]]->moveCount == 1)
					{
						if (currentIndex + 1 == 143 || currentIndex + 1 == 150)
						{
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex + 15;
							bEnPassant = true;
						}
					}
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
			if (BoardOfPieceIndexInVector[eatUp] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatUp]]->value > 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatUp;
					else
						EatableHashKeys[eatUp] = eatUp;
				}
			}
		}
		// since there's no eating backwards so we don't have to compare with leftThreshold
		if (eatDown <= eatDownRightThreshold && eatDown >= 0)
		{
			if (BoardOfPieceIndexInVector[eatDown] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatDown]]->value > 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatDown;
					else
						EatableHashKeys[eatDown] = eatDown;
				}
			}
		}
	}

	else if (attackerTurn == 3)
	{ // for top team - POSITIVE
		/* THE MOVE TO MOVE + EN PASSANT */
		// TODO: LATER IF WE WANT TO ADD MOVING BACK FROM FRIEND TEAM, THEN WE HAVE TO USE TIGHT CONDITION
		const int16 oneRowDownLeftThreshold = LeftThresholdOfIndex(currentIndex) - 14;
		const int16 oneRowDownRightThreshold = oneRowDownLeftThreshold + 13;
		if (currentIndex - 14 >= oneRowDownLeftThreshold && currentIndex - 14 <= oneRowDownRightThreshold)
		{
			if (BoardOfPieceIndexInVector[currentIndex-14] != -2)
			{
				/* THE MOVE TO MOVE */
				if (BoardOfPieceIndexInVector[currentIndex - 14] == -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex - 14;
				}
				// EN PASSANT:
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 14]]->value == -1)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 14]]->moveCount == 1)
					{
						if (currentIndex - 14 == 143 || currentIndex - 14 == 45)
						{
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex - 15;
							bEnPassant = true;
						}
					}
				}
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 14]]->value == -11)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 14]]->moveCount == 1)
					{
						if (currentIndex - 14 == 150 || currentIndex - 14 == 52)
						{
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex - 13;
							bEnPassant = true;
						}
					}
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
			if (BoardOfPieceIndexInVector[eatRight] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatRight]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatRight;
					else
						EatableHashKeys[eatRight] = eatRight;
				}
			}
		}
		if (eatLeft >= eatLeftThreshold)
		{
			if (BoardOfPieceIndexInVector[eatLeft] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatLeft]]->value < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatLeft;
					else
						EatableHashKeys[eatLeft] = eatLeft;
				}
			}
		}
	}

	else if (attackerTurn == -4)
	{ // for right team - NEGATIVE
		const int16 moveLeftThreshold = LeftThresholdOfIndex(currentIndex);
		/* THE MOVE TO MOVE */
		if (currentIndex - 1 > moveLeftThreshold)
		{
			if (BoardOfPieceIndexInVector[currentIndex - 1] != -2)
			{
				if (BoardOfPieceIndexInVector[currentIndex - 1] == -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = currentIndex - 1;
				}
				// EN PASSANT: ANALYZE ENEMY PAWN FIRST MOVE STATUS
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 1]]->value == 1)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 1]]->moveCount == 1)
					{
						if (currentIndex - 1 == 53 || currentIndex - 1 == 45)
						{
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex - 15;
							bEnPassant = true;
						}
					}
				}
				else if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 1]]->value == 11)
				{
					if (PiecesVector[BoardOfPieceIndexInVector[currentIndex - 1]]->moveCount == 1)
					{
						if (currentIndex - 1 == 150 || currentIndex - 1 == 143)
						{
							if (bUseHash == false)
								MovableIndices[ActiveIndex++] = currentIndex + 13;
							bEnPassant = true;
						}
					}
				}
			}
		}
		/*  THE MOVE TO EAT */
		const int16 eatUp = currentIndex + 13;
		const int16 eatDown = currentIndex - 15;
		const int16 eatUpLeftThreshold = LeftThresholdOfIndex(eatUp);
		const int16 eatDownLeftThreshold = LeftThresholdOfIndex(eatDown);

		if (eatUp >= eatUpLeftThreshold && eatUpLeftThreshold < 196)
		{
			if (BoardOfPieceIndexInVector[eatUp] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatUp]]->value > 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatUp;
					else
						EatableHashKeys[eatUp] = eatUp;
				}
			}
		}
		if (eatDown >= 0 && eatDown >= eatDownLeftThreshold)
		{
			if (BoardOfPieceIndexInVector[eatDown] >= 0)
			{
				if (PiecesVector[BoardOfPieceIndexInVector[eatDown]]->value > 0 && BoardOfPieceIndexInVector[eatDown] != -1)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = eatDown;
					else
						EatableHashKeys[eatDown] = eatDown;
				}
			}
		}
	}

	/* FIRST MOVE SPECIALS */
	// if the current piece having first move then it's guaranteed to be valid, so we don't have to validate with condition check again
	if (bUseHash == false)
	{
		if (PiecesVector[BoardOfPieceIndexInVector[currentIndex]]->moveCount == 0) // FIRST MOVE
		{
			if (attackerTurn == 1)
			{ // FOR BOTTOM TEAM ONLY
				if (BoardOfPieceIndexInVector[currentIndex + 14] == -1 && BoardOfPieceIndexInVector[currentIndex + 28] == -1)
					MovableIndices[ActiveIndex++] = currentIndex + 28;
			}
			else if (attackerTurn == -2)
			{ // FOR LEFT TEAM ONLY
				if (BoardOfPieceIndexInVector[currentIndex + 1] == -1 && BoardOfPieceIndexInVector[currentIndex + 2] == -1)
					MovableIndices[ActiveIndex++] = currentIndex + 2;
			}
			else if (attackerTurn == 3)
			{ // FOR LEFT TEAM ONLY
				if (BoardOfPieceIndexInVector[currentIndex - 14] == -1 && BoardOfPieceIndexInVector[currentIndex - 28] == -1)
					MovableIndices[ActiveIndex++] = currentIndex - 28;
			}
			else if (attackerTurn == -4)
			{ // FOR LEFT TEAM ONLY
				if (BoardOfPieceIndexInVector[currentIndex - 1] == -1 && BoardOfPieceIndexInVector[currentIndex - 2] == -1)
					MovableIndices[ActiveIndex++] = currentIndex - 2;
			}
		}
	}
	//for (int i = 0; i < ActiveIndex; i++) 
	//	UE_LOG(LogTemp, Warning, TEXT("Movable index: %d"), MovableIndices[i]);
}

// TODO: Fix this * attackerTurn as its logic is error with checkMateToWholeTeamOfPlayerNumber (check other team movables on our Turn)
void ASpawnManager::GetMovableIndicesRook(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerTurn)
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
	while (goLeft >= LeftThreshold && BoardOfPieceIndexInVector[goLeft] != -2)
	{
		if (BoardOfPieceIndexInVector[goLeft] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goLeft;
			else
				EatableHashKeys[goLeft] = goLeft;
			goLeft--;
		}
		else
		{
			// if reach one of our side then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goLeft]]->value * attackerTurn > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goLeft]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goLeft;
				else
					EatableHashKeys[goLeft] = goLeft;
				break;
			}
		}
	}

	while (goRight <= RightThreshold && BoardOfPieceIndexInVector[goRight] != -2)
	{
		if (BoardOfPieceIndexInVector[goRight] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goRight;
			else
				EatableHashKeys[goRight] = goRight;
			goRight++;
		}
		else
		{
			// if reach one of our side then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goRight]]->value * attackerTurn > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goRight]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goRight;
				else
					EatableHashKeys[goRight] = goRight;
				break;
			}
		}
	}

	while (goUp <= 195 && BoardOfPieceIndexInVector[goUp] != -2)
	{
		if (BoardOfPieceIndexInVector[goUp] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goUp;
			else
				EatableHashKeys[goUp] = goUp;
			goUp += 14;
		}
		else
		{
			// if reach one of our side then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goUp]]->value * attackerTurn > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goUp]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goUp;
				else
					EatableHashKeys[goUp] = goUp;
				break;
			}
		}
	}

	while (goDown >= 0 && BoardOfPieceIndexInVector[goDown] != -2)
	{
		if (BoardOfPieceIndexInVector[goDown] == -1)
		{
			if (bUseHash == false)
				MovableIndices[ActiveIndex++] = goDown;
			else
				EatableHashKeys[goDown] = goDown;
			goDown -= 14;
		}
		else
		{ 
			// if reach one of our side then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goDown]]->value * attackerTurn > 0)
				break;
			// if reach enemy side then take that 1 index into account then break instantly
			if (PiecesVector[BoardOfPieceIndexInVector[goDown]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goDown;
				else
					EatableHashKeys[goDown] = goDown;
				break;
			}
		}
	}
}

// TODO: Check all functions for SIGSEV safe with this new implentation (index != -1 and -2 before passing into pieces)
void ASpawnManager::GetMovableIndicesKnight(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerTurn)
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

	//UE_LOG(LogTemp, Warning, TEXT("current Row left thres: %d"), currentRowLeftThreshold);
	//UE_LOG(LogTemp, Warning, TEXT("current Row right thres: %d"), currentRowRightThreshold);
	//UE_LOG(LogTemp, Warning, TEXT("L1RowDownRight: %d"), L1RowDownRight);
	//UE_LOG(LogTemp, Warning, TEXT("L1RowUpRight: %d"), L1RowUpRight);
	//UE_LOG(LogTemp, Warning, TEXT("oneRowUpRight Threshold: %d"), oneRowUpRightThreshold);
	//UE_LOG(LogTemp, Warning, TEXT("oneRowUpLeft Threshold: %d"), oneRowUpLeftThreshold);

	// all are non-strict condition
	if (twoRowUpRightThreshold <= 195 && L2RowUpRight <= twoRowUpRightThreshold)
	{
		if (BoardOfPieceIndexInVector[L2RowUpRight] != -2)
		{
			if (BoardOfPieceIndexInVector[L2RowUpRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpRight;
				else
					EatableHashKeys[L2RowUpRight] = L2RowUpRight;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L2RowUpRight]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpRight;
				else
					EatableHashKeys[L2RowUpRight] = L2RowUpRight;
			}
		}
	}

	if (twoRowUpLeftThreshold <= 195 && L2RowUpLeft >= twoRowUpLeftThreshold)
	{
		if (BoardOfPieceIndexInVector[L2RowUpLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[L2RowUpLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpLeft;
				else
					EatableHashKeys[L2RowUpLeft] = L2RowUpLeft;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L2RowUpLeft]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowUpLeft;
				else
					EatableHashKeys[L2RowUpLeft] = L2RowUpLeft;
			}
		}
	}

	if (oneRowUpRightThreshold <= 195 && L1RowUpRight <= oneRowUpRightThreshold)
	{
		if (BoardOfPieceIndexInVector[L1RowUpRight] != -2)
		{
			if (BoardOfPieceIndexInVector[L1RowUpRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpRight;
				else
					EatableHashKeys[L1RowUpRight] = L1RowUpRight;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L1RowUpRight]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpRight;
				else
					EatableHashKeys[L1RowUpRight] = L1RowUpRight;
			}
		}
	}

	if (oneRowUpLeftThreshold <= 195 && L1RowUpLeft >= oneRowUpLeftThreshold)
	{
		if (BoardOfPieceIndexInVector[L1RowUpLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[L1RowUpLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpLeft;
				else
					EatableHashKeys[L1RowUpLeft] = L1RowUpLeft;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L1RowUpLeft]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowUpLeft;
				else
					EatableHashKeys[L1RowUpLeft] = L1RowUpLeft;
			}
		}
	}
	/* DOWNWARDS */
	if (L2RowDownRight >= 0 && L2RowDownRight <= twoRowDownRightThreshold)
	{
		if (BoardOfPieceIndexInVector[L2RowDownRight] != -2)
		{
			if (BoardOfPieceIndexInVector[L2RowDownRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownRight;
				else
					EatableHashKeys[L2RowDownRight] = L2RowDownRight;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L2RowDownRight]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownRight;
				else
					EatableHashKeys[L2RowDownRight] = L2RowDownRight;
			}
		}
	}
	if (twoRowDownLeftThreshold >= 0 && L2RowDownLeft >= twoRowDownLeftThreshold)
	{
		if (BoardOfPieceIndexInVector[L2RowDownLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[L2RowDownLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownLeft;
				else
					EatableHashKeys[L2RowDownLeft] = L2RowDownLeft;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L2RowDownLeft]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L2RowDownLeft;
				else
					EatableHashKeys[L2RowDownLeft] = L2RowDownLeft;
			}
		}
	}
	if (L1RowDownRight >= 0 && L1RowDownRight <= oneRowDownRightThreshold)
	{
		if (BoardOfPieceIndexInVector[L1RowDownRight] != -2)
		{
			if (BoardOfPieceIndexInVector[L1RowDownRight] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownRight;
				else
					EatableHashKeys[L1RowDownRight] = L1RowDownRight;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L1RowDownRight]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownRight;
				else
					EatableHashKeys[L1RowDownRight] = L1RowDownRight;
			}
		}
	}

	if (oneRowDownLeftThreshold >= 0 && L1RowDownLeft >= oneRowDownLeftThreshold)
	{
		if (BoardOfPieceIndexInVector[L1RowDownLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[L1RowDownLeft] == -1)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownLeft;
				else
					EatableHashKeys[L1RowDownLeft] = L1RowDownLeft;
			}
			else if (PiecesVector[BoardOfPieceIndexInVector[L1RowDownLeft]]->value * attackerTurn < 0)
			{
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = L1RowDownLeft;
				else
					EatableHashKeys[L1RowDownLeft] = L1RowDownLeft;
			}
		}
	}

	//for (int i = 0; i < ActiveIndex; i++)
	//	UE_LOG(LogTemp, Warning, TEXT("Indices for knights: %d"), MovableIndices[i]);

}

// TODO:  MUST DO THE TIGHT THRESHOLD FOR THIS! 
void ASpawnManager::GetMovableIndicesBishop(int16& storageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerTurn)
{
	int16 goUpLeft = currentIndex + 13,
		goUpRight = currentIndex + 15,
		goDownLeft = currentIndex - 15,
		goDownRight = currentIndex - 13;

	ActiveIndex = storageIndex;

	if (currentIndex % 14 != 0)
	{
		while (goUpLeft <= 195 && BoardOfPieceIndexInVector[goUpLeft] != -2)
		{ // this is not detecting the enemy queen value and break?
			if (BoardOfPieceIndexInVector[goUpLeft] == -1)
			{
				// TODO: we can think of going until column 0 (in 2D sense) could be translated into 1D as % 14 == 0? And we can use that as the tight threshold (instead of having to use real tight threshold)
				if (goUpLeft % 14 == 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = goUpLeft;
					else
						EatableHashKeys[goUpLeft] = goUpLeft;
					break;
				}
				//UE_LOG(LogTemp, Warning, TEXT("goUpLeft: Index to Add: %d"), goUpLeft);
				// TODO: APPLY THIS 'USE HASH' FOR ALL THE GET MOVABLES FUNCTIONS BELOW
				if (bUseHash == false)
					// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
					MovableIndices[ActiveIndex++] = goUpLeft;
				else
					EatableHashKeys[goUpLeft] = goUpLeft;
				// this is wrongly calculated leading to 97 from '84' + 13 (end of board for queen to go UP left, overflow to 97 after + 13)
				goUpLeft = goUpLeft + 13;
			}
			else
			{ /// these are tighter conditions than the goUpRight % 14 == 0 in case of board index != -1, thus we don't have to overlay that condition on top
				// TODO: we get one more index of the enemy team into movables and ONLY AFTERWARDS do we break
				if (PiecesVector[BoardOfPieceIndexInVector[goUpLeft]]->value * attackerTurn > 0)
					break;
				if (PiecesVector[BoardOfPieceIndexInVector[goUpLeft]]->value * attackerTurn < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = goUpLeft;
					else
						EatableHashKeys[goUpLeft] = goUpLeft;
					break;
				}
			}
		}
		while (goDownLeft >= 0 && BoardOfPieceIndexInVector[goDownLeft] != -2)
		{ // same mindset as above
			if (BoardOfPieceIndexInVector[goDownLeft] == -1)
			{
				if (goDownLeft % 14 == 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = goDownLeft;
					else
						EatableHashKeys[goDownLeft] = goDownLeft;
					break;
				}
				//UE_LOG(LogTemp, Warning, TEXT("goDownLeft: Index to Add: %d"), goDownLeft);
				// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goDownLeft;
				else
					EatableHashKeys[goDownLeft] = goDownLeft;
				goDownLeft = goDownLeft - 15;
			}
			else
			{
				if (PiecesVector[BoardOfPieceIndexInVector[goDownLeft]]->value * attackerTurn > 0)
					break;
				if (PiecesVector[BoardOfPieceIndexInVector[goDownLeft]]->value * attackerTurn < 0)
				{
					if (bUseHash == false)
						// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
						MovableIndices[ActiveIndex++] = goDownLeft;
					else
						EatableHashKeys[goDownLeft] = goDownLeft;
					break;
				}
			}
		}
	}

	if (currentIndex % 14 != 13)
	{
		while (goUpRight <= 195 && BoardOfPieceIndexInVector[goUpRight] != -2)
		{ // similar mindset as above, we can use % 14 == 13 as the right threshold of reaching end column
			if (BoardOfPieceIndexInVector[goUpRight] == -1)
			{
				if (goUpRight % 14 == 13)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = goUpRight;
					else
						EatableHashKeys[goUpRight] = goUpRight;
					break;
				}
				//UE_LOG(LogTemp, Warning, TEXT("goUpRight: Index to Add: %d"), goUpRight);
				// go up and shift left 1 unit relatively to current position (which already shift left 1 unit compared to the position before it - meaning that total deviation compared to original position already increases)
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goUpRight;
				else
					EatableHashKeys[goUpRight] = goUpRight;
				goUpRight = goUpRight + 15;
			}
			else
			{ /// these are tighter conditions than the goUpRight % 14 == 13 in case of board index != -1, thus we don't have to overlay that condition on top
				if (PiecesVector[BoardOfPieceIndexInVector[goUpRight]]->value * attackerTurn > 0)
					break;
				if (PiecesVector[BoardOfPieceIndexInVector[goUpRight]]->value * attackerTurn < 0)
				{
					if (bUseHash == false)
						// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
						MovableIndices[ActiveIndex++] = goUpRight;
					else
						EatableHashKeys[goUpRight] = goUpRight;
					break;
				}
			}
		}
		while (goDownRight >= 0 && BoardOfPieceIndexInVector[goDownRight] != -2)
		{
			if (BoardOfPieceIndexInVector[goDownRight] == -1)
			{
				if (goDownRight % 14 == 13)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = goDownRight;
					else
						EatableHashKeys[goDownRight] = goDownRight;
					break;
				}
				//UE_LOG(LogTemp, Warning, TEXT("goDownRight: Index to Add: %d"), goDownRight);
				// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
				if (bUseHash == false)
					MovableIndices[ActiveIndex++] = goDownRight;
				else
					EatableHashKeys[goDownRight] = goDownRight;
				goDownRight = goDownRight - 13;
			}
			else
			{
				if (PiecesVector[BoardOfPieceIndexInVector[goDownRight]]->value * attackerTurn > 0)
					break;
				if (PiecesVector[BoardOfPieceIndexInVector[goDownRight]]->value * attackerTurn < 0)
				{
					if (bUseHash == false)
						MovableIndices[ActiveIndex++] = goDownRight;
					else
						EatableHashKeys[goDownRight] = goDownRight;
					break;
				}
			}
		}
	}
}

void ASpawnManager::GetMovableIndicesQueen(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerTurn)
{
	GetMovableIndicesBishop(startStorageIndex, currentIndex, bUseHash, attackerTurn);
	GetMovableIndicesRook(ActiveIndex, currentIndex, bUseHash, attackerTurn);
}

/* TODO: THIS ONE NOT HAVING TIGHT THRESHOLD,  MIGHT CAUSE IT SEG FAULT, CHAINING THE SEGFAULT TO CHECKMATE? */
void ASpawnManager::GetMovableIndicesKing(int16& startStorageIndex, const uint8& currentIndex, bool bUseHash, const int16& attackerTurn)
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
		if (kingUpLeft <= 195 && kingUpLeft >= OneRowUpLeftThreshold && BoardOfPieceIndexInVector[kingUpLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[kingUpLeft] == -1)
				MovableIndices[ActiveIndex++] = kingUpLeft;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingUpLeft]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingUpLeft;
		}
		if (kingUp <= 195 && BoardOfPieceIndexInVector[kingUp] != -2)
		{
			if (BoardOfPieceIndexInVector[kingUp] == -1)
				MovableIndices[ActiveIndex++] = kingUp;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingUp]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingUp;
		}
		if (OneRowUpRightThreshold <= 195 && kingUpRight <= OneRowUpRightThreshold && BoardOfPieceIndexInVector[kingUpRight] != -2)
		{
			if (BoardOfPieceIndexInVector[kingUpRight] == -1)
				MovableIndices[ActiveIndex++] = kingUpRight;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingUpRight]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingUpRight;
		}
		if (kingLeft >= currentRowLeftThreshold && BoardOfPieceIndexInVector[kingLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[kingLeft] == -1)
				MovableIndices[ActiveIndex++] = kingLeft;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingLeft]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingLeft;
		}
		if (kingRight <= currentRowRightThreshold && BoardOfPieceIndexInVector[kingRight] != -2)
		{
			if (BoardOfPieceIndexInVector[kingRight] == -1)
				MovableIndices[ActiveIndex++] = kingRight;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingRight]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingRight;
		}
		if (OneRowDownLeftThreshold >= 0 && kingDownLeft >= OneRowDownLeftThreshold && BoardOfPieceIndexInVector[kingDownLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[kingDownLeft] == -1)
				MovableIndices[ActiveIndex++] = kingDownLeft;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingDownLeft]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingDownLeft;
		}
		if (kingDown >= 0 && BoardOfPieceIndexInVector[kingDown] != -2)
		{
			if (BoardOfPieceIndexInVector[kingDown] == -1)
				MovableIndices[ActiveIndex++] = kingDown;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingDown]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingDown;
		}
		if (kingDownRight >= 0 && kingDownRight <= OneRowDownRightThreshold && BoardOfPieceIndexInVector[kingDownRight] != -2)
		{
			if (BoardOfPieceIndexInVector[kingDownRight] == -1)
				MovableIndices[ActiveIndex++] = kingDownRight;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingDownRight]]->value * attackerTurn < 0)
				MovableIndices[ActiveIndex++] = kingDownRight;
		}
	}
	else
	{ // PUSH INTO HASH KEYS
		if (kingUpLeft <= 195 && kingUpLeft >= OneRowUpLeftThreshold && BoardOfPieceIndexInVector[kingUpLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[kingUpLeft] == -1)
				EatableHashKeys[kingUpLeft] = kingUpLeft;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingUpLeft]]->value * attackerTurn < 0)
				EatableHashKeys[kingUpLeft] = kingUpLeft;
		}
		if (kingUp <= 195 && BoardOfPieceIndexInVector[kingUp] != -2)
		{
			if (BoardOfPieceIndexInVector[kingUp] == -1)
				EatableHashKeys[kingUp] = kingUp;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingUp]]->value * attackerTurn < 0)
				EatableHashKeys[kingUp] = kingUp;
		}
		if (OneRowUpRightThreshold <= 195 && kingUpRight <= OneRowUpRightThreshold && BoardOfPieceIndexInVector[kingUpRight] != -2)
		{
			if (BoardOfPieceIndexInVector[kingUpRight] == -1)
				EatableHashKeys[kingUpRight] = kingUpRight;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingUpRight]]->value * attackerTurn < 0)
				EatableHashKeys[kingUpRight] = kingUpRight;
		}
		if (kingLeft >= currentRowLeftThreshold && BoardOfPieceIndexInVector[kingLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[kingLeft] == -1)
				EatableHashKeys[kingLeft] = kingLeft;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingLeft]]->value * attackerTurn < 0)
				EatableHashKeys[kingLeft] = kingLeft;
		}
		if (kingRight <= currentRowRightThreshold && BoardOfPieceIndexInVector[kingRight] != -2)
		{
			if (BoardOfPieceIndexInVector[kingRight] == -1)
				EatableHashKeys[kingRight] = kingRight;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingRight]]->value * attackerTurn < 0)
				EatableHashKeys[kingRight] = kingRight;
		}
		if (OneRowDownLeftThreshold >= 0 && kingDownLeft >= OneRowDownLeftThreshold && BoardOfPieceIndexInVector[kingDownLeft] != -2)
		{
			if (BoardOfPieceIndexInVector[kingDownLeft] == -1)
				EatableHashKeys[kingDownLeft] = kingDownLeft;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingDownLeft]]->value * attackerTurn < 0)
				EatableHashKeys[kingDownLeft] = kingDownLeft;
		}
		if (kingDown >= 0 && BoardOfPieceIndexInVector[kingDown] != -2)
		{
			if (BoardOfPieceIndexInVector[kingDown] == -1)
				EatableHashKeys[kingDown] = kingDown;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingDown]]->value * attackerTurn < 0)
				EatableHashKeys[kingDown] = kingDown;
		}
		if (kingDownRight >= 0 && kingDownRight <= OneRowDownRightThreshold && BoardOfPieceIndexInVector[kingDownRight] != -2)
		{
			if (BoardOfPieceIndexInVector[kingDownRight] == -1)
				EatableHashKeys[kingDownRight] = kingDownRight;
			else if (PiecesVector[BoardOfPieceIndexInVector[kingDownRight]]->value * attackerTurn < 0)
				EatableHashKeys[kingDownRight] = kingDownRight;
		}
	}

	// TODO: Check if the current King is qualified for Castling, if yes then highlight that as one of the movable
	// TODO: add this Castling case to the makeMove(), remmeber to update the moveCount status of both to 0 after Castling (we have already done that)
	// TODO: Castling is special because it's doing 2 moves at once, thus think of how you do this in makeMove()
	if (bUseHash == false) // not to mess up with Eatable Indices and checkMateToWholeTeamOfPlayerNumber()
	{
		if (attackerTurn == 1)
		{ // check if King is being checked
			if (bP1Checked == false)
			{ // check if King is still first move
				if (PiecesVector[15]->moveCount == 0)
				{ 
					// get all the eatable moves for later check of checkMate on road
					EatableHashKeys.Init(-1, 196);
					checkMateToWholeTeamOfPlayerNumber(1);
					/* RIGHT SIDE CASTLING - SHORT */
					if (PiecesVector[9]->moveCount == 0)
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[8] == -1 && BoardOfPieceIndexInVector[9] == -1)
						{
							if (EatableHashKeys[8] != 8 && EatableHashKeys[9] != 9) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 9;
						}
					}
					/* LEFT SIDE CASTLING - LONG */
					if (PiecesVector[8]->moveCount == 0)	// left side Rook is first move
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[6] == -1 && BoardOfPieceIndexInVector[5] == -1 && BoardOfPieceIndexInVector[4] == -1)
						{
							if (EatableHashKeys[4] != 4 && EatableHashKeys[5] != 5 && EatableHashKeys[6] != 6) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 5;
						}
					}
				}
			}
			// reset eatable hash keys at end
			EatableHashKeys.Init(-1, 196);
			// assign Castling Status to true
			bP1Castling = true;
		}

		else if (attackerTurn == -2)
		{ // check if King is being checked
			if (bP2Checked == false)
			{ // check if King is still first move
				if (PiecesVector[47]->moveCount == 0)
				{ 
					// get all the eatable moves for later check of checkMate on road
					EatableHashKeys.Init(-1, 196);
					checkMateToWholeTeamOfPlayerNumber(-2);
					/* TOWARD TOP CASTLING - SHORT */
					if (PiecesVector[41]->moveCount == 0)
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[112] == -1 && BoardOfPieceIndexInVector[126] == -1)
						{
							if (EatableHashKeys[112] != 112 && EatableHashKeys[126] != 126) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 126;
						}
					}
					/* TOWARD BOTTOM CASTLING - LONGER CASTLING ROUTE */
					if (PiecesVector[40]->moveCount == 0)
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[84] == -1 && BoardOfPieceIndexInVector[70] == -1 && BoardOfPieceIndexInVector[56] == -1)
						{
							if (EatableHashKeys[84] != 84 && EatableHashKeys[70] != 70 && EatableHashKeys[56] != 56) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 70;
						}
					}
				}
			}
			// reset eatable hash keys at end
			EatableHashKeys.Init(-1, 196);
			bP2Castling = true;
		}

		else if (attackerTurn == 3)
		{ // check if King is being checked
			if (bP3Checked == false)
			{ // check if King is still first move
				if (PiecesVector[31]->moveCount == 0)
				{ 
					// get all the eatable moves for later check of checkMate on road
					EatableHashKeys.Init(-1, 196);
					checkMateToWholeTeamOfPlayerNumber(3);
					/* LEFT CASTLING - SHORT */
					if (PiecesVector[24]->moveCount == 0) // check if rook still first move
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[186] == -1 && BoardOfPieceIndexInVector[187] == -1)
						{
							if (EatableHashKeys[186] != 186 && EatableHashKeys[187] != 187) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 186;
						}
					}
					/* Right CASTLING - Longer Castling Route */
					if (PiecesVector[25]->moveCount == 0)
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[189] == -1 && BoardOfPieceIndexInVector[190] == -1 && BoardOfPieceIndexInVector[191] == -1)
						{
							if (EatableHashKeys[189] != 189 && EatableHashKeys[190] != 190 && EatableHashKeys[191] != 191) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 190;
						}
					}
				}
			}
			// reset eatable hash keys at end
			EatableHashKeys.Init(-1, 196);
			bP3Castling = true;
		}

		else if (attackerTurn == -4)
		{ // check if King is being checked
			if (bP4Checked == false)
			{ // check if King is still first move
				if (PiecesVector[63]->moveCount == 0)
				{ 
					// get all the eatable moves for later check of checkMate on road
					EatableHashKeys.Init(-1, 196);
					checkMateToWholeTeamOfPlayerNumber(-4);
					/* TOWARD BOTTOM CASTLING - SHORTER CASTLING ROUTE */
					if (PiecesVector[56]->moveCount == 0)
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[83] == -1 && BoardOfPieceIndexInVector[69] == -1)
						{
							if (EatableHashKeys[83] != 83 && EatableHashKeys[69] != 69) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 69;
						}
					}
					/* TOWARD TOP CASTLING - Longer Castling Route */
					if (PiecesVector[57]->moveCount == 0)
					{
						/// check if the road to castling are clear
						if (BoardOfPieceIndexInVector[111] == -1 && BoardOfPieceIndexInVector[125] == -1 && BoardOfPieceIndexInVector[139] == -1)
						{
							if (EatableHashKeys[111] != 111 && EatableHashKeys[125] != 125 && EatableHashKeys[139] != 139) // meaning that it doesn't contains these indices
								MovableIndices[ActiveIndex++] = 125;
						}
					}
				}
			}
			// reset eatable hash keys at end
			EatableHashKeys.Init(-1, 196);
			bP4Castling = true;
		}
	}

	// TODO: HUD for castling and Attack

	// DEBUG purpose
	//for (int i = 0; i < ActiveIndex; i++)
	//		UE_LOG(LogTemp, Warning, TEXT("King MovableIndices are: %d"), MovableIndices[i]);
}

// THESE THRESHOLD SHOULD BE COMPARED TO THE THRESHOLD OF THE WHOLE BOARD, SO THAT WE ARE NOT CONFUSE
// BOTH OF THESE FUNCTIONS ARE CLAMP WITHIN [0, 195]
int16 ASpawnManager::LeftThresholdOfIndex(const int16& index)
{
	return std::min(std::max(FMath::Floor(index / 14) * 14, 0), 195);
}

int16 ASpawnManager::RightThresholdOfIndex(const int16& index)
{
	return std::min(std::max(LeftThresholdOfIndex(index) + 13, 0), 195);
}

// VALIDATION OF CLICK 1
//this might not be as good as to change to storing one hit variable and compare the second click to the variable to see if we reset the click again 
bool ASpawnManager::isClick1P1IndexValid(const int16& hitIndexOnBoard)
{
	if (BoardOfPieceIndexInVector[hitIndexOnBoard] < 0 || BoardOfPieceIndexInVector[hitIndexOnBoard] > 15) return false;
	return isIndexValid(hitIndexOnBoard);
}

bool ASpawnManager::isClick1P3IndexValid(const int16& hitIndexOnBoard)
{
	if (BoardOfPieceIndexInVector[hitIndexOnBoard] < 16 || BoardOfPieceIndexInVector[hitIndexOnBoard] > 31) return false;
	return isIndexValid(hitIndexOnBoard);
}

bool ASpawnManager::isClick1P2IndexValid(const int16& hitIndexOnBoard)
{
	if (BoardOfPieceIndexInVector[hitIndexOnBoard] < 32 || BoardOfPieceIndexInVector[hitIndexOnBoard] > 47) return false;
	return isIndexValid(hitIndexOnBoard);
}

bool ASpawnManager::isClick1P4IndexValid(const int16& hitIndexOnBoard)
{
	if (BoardOfPieceIndexInVector[hitIndexOnBoard] < 48 || BoardOfPieceIndexInVector[hitIndexOnBoard] > 63) return false;
	return isIndexValid(hitIndexOnBoard);
}

// VALIDATION OF CLICK 2
// Because after a Valid Click 1 we've already got MovableIndices of that specific player and piece -> just need 1 check for all kinds of player
bool ASpawnManager::isClick2MoveIndexValid(const int16& hitIndexOnBoard)
{
	//UE_LOG(LogTemp, Warning, TEXT("hitIndexOnBoard: %d"), hitIndexOnBoard);
	//UE_LOG(LogTemp, Warning, TEXT("ActiveIndex: %d"), ActiveIndex);

	// see if the MovableIndices contains the hitIndexOnBoard
	if (ActiveIndex == 0) return false; // if no move available, we just mark as false so that it returns to waiting from click1
	// TODO: fix: see if movable indices WITHIN ACTIVE INDEX RANGE contains the value
	for (int i = 0; i < ActiveIndex; i++)
		if (hitIndexOnBoard == MovableIndices[i])
			return true;
	return false;
}

bool ASpawnManager::isIndexValid(const int16& index)
{
	return index > 0 && index < 196 && BoardOfPieceIndexInVector[index] != -2;
}

// Checking the type of piece
bool ASpawnManager::isPawn(const int16& pieceValue)
{
	return abs(pieceValue) == 1 || abs(pieceValue) == 11;
}


bool ASpawnManager::isRook(const int16& pieceValue)
{
	return abs(pieceValue) == 2 || abs(pieceValue) == 12;
}


bool ASpawnManager::isKnight(const int16& pieceValue)
{
	return abs(pieceValue) == 3 || abs(pieceValue) == 13;
}

bool ASpawnManager::isBishop(const int16& pieceValue)
{
	return abs(pieceValue) == 4 || abs(pieceValue) == 14;
}


bool ASpawnManager::isQueen(const int16& pieceValue)
{
	return abs(pieceValue) == 5 || abs(pieceValue) == 15;
}


bool ASpawnManager::isKing(const int16& pieceValue)
{
	return abs(pieceValue) == 6 || abs(pieceValue) == 16;
}

// TODO: we are going to set the material here instead of inside the pieces constructors
// TODO: SET THE PROPER MATERIALS FOR EACH SIDES
/* TOP */
void ASpawnManager::spawnBottomActors()
{
	uint8 pieceValue = 1;
	/* PAWN: 1 */
	float x = 600.f, y = 1400.f;
	for (int i = 0; i < 8; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		PiecesVector.Top()->SetMaterialInactive(1);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		//UE_LOG(LogTemp, Warning, (TEXT("indexOnboard: %d")), PiecesVector.Top()->indexOnBoard);
		y += 400;
	}
	++pieceValue;

	/* ROOK: 2 */
	x = 200.f; y = 1400.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		// some operations on tweaking the location here
		PiecesVector.Top()->SetMaterialInactive(1);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2800.f;
	}
	++pieceValue;

	/* KNIGHT: 3 */
	x = 200.f; y = 1800.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		PiecesVector.Top()->SetMaterialInactive(1);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2000.f;
	}
	++pieceValue;

	/* BISHOP: 4 */
	x = 200.f; y = 2200.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		PiecesVector.Top()->SetMaterialInactive(1);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 1200.f;
	}

	++pieceValue;

	/* QUEEN: 5 */
	x = 200.f; y = 2600.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	PiecesVector.Top()->SetMaterialInactive(1);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));

	++pieceValue;

	/* KING: 6 */
	x = 200.f; y = 3000.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	PiecesVector.Top()->SetMaterialInactive(1);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
}

/* LEFT */
void ASpawnManager::spawnLeftActors()
{
	int8 pieceValue = -1;
	/* PAWN: -1 */
	float x = 1400.f, y = 600.f;
	for (int i = 0; i < 8; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
		PiecesVector.Top()->SetMaterialInactive(2);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 400.f;
	}

	pieceValue--;

	/* ROOK: -2 */
	x = 1400.f; y = 200.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		PiecesVector.Top()->SetMaterialInactive(2);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2800.f;
	}
	pieceValue--;

	/* KNIGHT: -3 */
	x = 1800.f; y = 200.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
		PiecesVector.Top()->SetMaterialInactive(2);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2000.f;
	}
	pieceValue--;

	/* BISHOP: -4 */
	x = 2200.f; y = 200.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
		PiecesVector.Top()->SetMaterialInactive(2);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 1200.f;
	}
	pieceValue--;

	/* QUEEN */
	x = 2600.f; y = 200.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator(0, 90, 0)));
	PiecesVector.Top()->SetMaterialInactive(2);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = 84;

	pieceValue--;

	/* KING */
	x = 3000.f; y = 200.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	PiecesVector.Top()->SetMaterialInactive(2);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = 98;

}

/* TOP */
void ASpawnManager::spawnTopActors()
{
	uint8 pieceValue = 11;
	/* PAWN: 11 */
	float x = 5000.f, y = 1400.f;
	for (int i = 0; i < 8; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
		PiecesVector.Top()->SetMaterialInactive(3);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 400.f;
	}
	pieceValue++;

	/* ROOK: 12 */
	x = 5400.f; y = 1400.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		PiecesVector.Top()->SetMaterialInactive(3);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2800.f;
	}
	pieceValue++;

	/* KNIGHT: 13 */
	x = 5400.f; y = 1800.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
		PiecesVector.Top()->SetMaterialInactive(3);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 2000.f;
	}
	pieceValue++;

	/* BISHOP: 14 */
	x = 5400.f; y = 2200.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
		PiecesVector.Top()->SetMaterialInactive(3);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		y += 1200.f;
	}

	pieceValue++;

	/* QUEEN: 15 */
	x = 5400.f; y = 3000.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator(0, 180, 0)));
	PiecesVector.Top()->SetMaterialInactive(3);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = 189;

	pieceValue++;

	/* KING: 16 */
	x = 5400.f; y = 2600.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	PiecesVector.Top()->SetMaterialInactive(3);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = 188;

}

void ASpawnManager::spawnRightActors()
{
	int8 pieceValue = -11;
	/* PAWN */
	float x = 1400.f, y = 5000.f;
	for (int i = 0; i < 8; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<APawnActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
		PiecesVector.Top()->SetMaterialInactive(4);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 400.f;
	}
	pieceValue--;

	/* ROOK */
	x = 1400.f; y = 5400.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ARookActor>(FVector(x, y, 0), FRotator::ZeroRotator));
		PiecesVector.Top()->SetMaterialInactive(4);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2800.f;
	}
	pieceValue--;

	/* KNIGHT */
	x = 1800.f; y = 5400.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<AKnightActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
		PiecesVector.Top()->SetMaterialInactive(4);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 2000.f;
	}
	pieceValue--;

	/* BISHOP */
	x = 2200.f; y = 5400.f;
	for (int i = 0; i < 2; i++)
	{
		PiecesVector.Add(GetWorld()->SpawnActor<ABishopActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
		PiecesVector.Top()->SetMaterialInactive(4);
		PiecesVector.Top()->value = pieceValue;
		PiecesVector.Top()->indexOnBoard = LocationToIndex(FVector(x, y, 0));
		x += 1200.f;
	}
	pieceValue--;
	/* QUEEN */
	x = 3000.f; y = 5400.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AQueenActor>(FVector(x, y, 0), FRotator(0, -90, 0)));
	PiecesVector.Top()->SetMaterialInactive(4);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = 111;

	pieceValue--;
	/* KING */
	x = 2600.f; y = 5400.f;
	PiecesVector.Add(GetWorld()->SpawnActor<AKingActor>(FVector(x, y, 0), FRotator::ZeroRotator));
	PiecesVector.Top()->SetMaterialInactive(4);
	PiecesVector.Top()->value = pieceValue;
	PiecesVector.Top()->indexOnBoard = 97;

}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	Board = GetWorld()->SpawnActor<AChessBoard>();

	// same team spawn next to each other 
	spawnBottomActors();
	spawnTopActors();
	spawnLeftActors();
	spawnRightActors();

    if (HasAuthority()) {
        for (int i = 0; i < PiecesVector.Num(); i++)
        {
            UE_LOG(LogTemp, Warning, TEXT("Value in array: %d. First move status: %d. Index On Board: %d"), PiecesVector[i]->value, PiecesVector[i]->moveCount, PiecesVector[i]->indexOnBoard);
        }
    }

    AActor* spawnOwner = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (spawnOwner) 
    {
        UE_LOG(LogTemp, Warning, TEXT("New own not NULL"));
        SetOwner(spawnOwner);
    }


}

void ASpawnManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	
}
