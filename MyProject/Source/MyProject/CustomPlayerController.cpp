// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "ChessBoard.h"
#include "ChessPieces.h"

#define OUT

#define BOTTOM 1
#define LEFT 2
#define TOP 3
#define RIGHT 4

// Currently non-compilable as the functions are not completed yet
ACustomPlayerController::ACustomPlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;

	Board = nullptr;
	Pieces = nullptr;
	clickCount = 0;
	hitIndex1 = -1;
	// init all the mechanics arrays and variables here
	// remember to exchange the value of the mechanic array whenever you move
	ActiveIndex = 0;
	bEnPassant = false;
	MovableIndices.SetNum(196, false);
	ChessBoardValues = {
		 7,  7,  7,  2,  3,  4,  5,  6,  4,  3,  2,  7,  7,  7,
		 7,  7,  7,  1,  1,  1,  1,  1,  1,  1,  1,  7,  7,  7,
		 7,  7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  7,
		-2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-12,
		-3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-13,
		-4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-14,
		-5, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-15,
		-6, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-16,
		-4, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-14,
		-3, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-13,
		-2, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,-11,-12,
		 7,  7,  7,  0,  0,  0,  0,  0,  0,  0,  0,  7,  7,  7,
		 7,  7,  7, 11, 11, 11, 11, 11, 11, 11, 11,  7,  7,  7,
		 7,  7,  7, 12, 13, 14, 15, 16, 14, 13, 12,  7,  7,  7
	};

	ISMIndexMapOn196Board = {
		-1, -1, -1, 1, 1, 1, 1, 1, 2, 2, 2, -1, -1, -1,
		-1, -1, -1, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		5, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, 7,
		5, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, 7,
		5, 19, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, 7,
		3, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, 4,
		3, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, 4,
		6, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, 8,
		6, 23, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, 8,
		6, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, 8,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, 9, 10, 11, 12, 13, 14, 15, 16, -1, -1, -1,
		-1, -1, -1, 3, 3, 3, 2, 2, 4, 4, 4, -1, -1, -1	
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

// TODO: this will be the makeMove() function, as we use 2 left-mouse clicks for moving from A to B
void ACustomPlayerController::OnLeftMouseClick()
{
	FHitResult TraceResult(ForceInit);
	// might have to change the collision Channel to Visible, because Our objects are in 1 mesh (thus cannot detect the different type of ECC for each object)
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, OUT TraceResult);
	FVector hitLocation = TraceResult.Location;

	// the events happening in Click1
	if (clickCount == 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Left mouse click 1 is triggered!"));
		UE_LOG(LogTemp, Warning, TEXT("current clickCount is %d"), clickCount);
		//if (player1) 
		{
			hitIndex1 = LocationToIndex(hitLocation);
			if (!isClick1P1IndexValid(hitIndex1)) return;
		} 
/* 
		//if (player2) 
		{
			hitLocation = TraceResult.Location;
			hitIndex1 = LocationToIndex(hitLocation);
			if (!isClick1P2IndexValid(hitIndex1)) return;
		} 
		//if (player3) 
		{
			hitLocation = TraceResult.Location;
			hitIndex1 = LocationToIndex(hitLocation);
			if (!isClick1P3IndexValid(hitIndex1)) return;
		} 
		//if (player4) 
		{
			hitLocation = TraceResult.Location;
			hitIndex1 = LocationToIndex(hitLocation);
			if (!isClick1P4IndexValid(hitIndex1)) return;
		} 
*/
		clickCount = 1;
		const int32 hitValue1 = ChessBoardValues[hitIndex1];
		int32 startStorageIndex = 0;
		HighlightSelectedTileByIndex(hitIndex1);

		UE_LOG(LogTemp, Warning, TEXT("Hit Value is %d!"), ChessBoardValues[hitIndex1]);
		UE_LOG(LogTemp, Warning, TEXT("Hit Index is %d!"), hitIndex1);
		if (isPawn(hitValue1)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
			GetMovableIndicesPawn(startStorageIndex, hitLocation);
			HighlightMovableIndices(0);
		}
		else if (isRook(hitValue1)) 
		{
			GetMovableIndicesRook(startStorageIndex, hitLocation);
			HighlightMovableIndices(0);
		}
		else if (isKnight(hitValue1)) 
		{
			GetMovableIndicesKnight(startStorageIndex, hitLocation);
			HighlightMovableIndices(0);
		}
		else if (isBishop(hitValue1)) 
		{
			GetMovableIndicesBishop(startStorageIndex, hitLocation);
			HighlightMovableIndices(0);
		}
		else if (isQueen(hitValue1)) 
		{
			GetMovableIndicesQueen(startStorageIndex, hitLocation);
			HighlightMovableIndices(0);
		}
		else if (isKing(hitValue1))  
		{
			GetMovableIndicesKing(startStorageIndex, hitLocation);
			HighlightMovableIndices(0);
		}
		return;
	}

	// TODO: MIGHT HAVE TO CHANGE THE CONDITION CHECKING OF CLICK1 AND CLICK2 TO EXCLUDE 0 AND MAKE IT A SEPARATE CHECK
	///TODO: CHECK IF INDEX IS THE SAME, if the same then we repeat 1 + unhighlight the last selected
	// the events happening in click 2
	int32 hitIndex2;
	if (clickCount == 1) 
	{ 
		hitIndex2 = LocationToIndex(hitLocation);
		UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is triggered!"));
		UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is at index: %d!"), hitIndex2);
		if (!isClick2MoveIndexValid(hitIndex2)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is not valid at %d with value %d!"), hitIndex2, ChessBoardValues[hitIndex2]);
			// if it's the  player click again on one of his piece(which means click1 is valid), we cast it back to click1
			if (isClick1P1IndexValid(hitIndex2)) 
			{
				UE_LOG(LogTemp,Warning,TEXT("Cast back to click1 from click2"));
				// unhighlight all the past highlighted tiles
				for (int i = 0; i < ActiveIndex - 1 ; i++) 
					UnhighlightTileByIndex(i, false);
				UnhighlightTileByIndex(ActiveIndex - 1, true);
				UnhighlightTileByIndex(hitIndex1, true);

				const int32 hitValue2 = ChessBoardValues[hitIndex2];
				HighlightSelectedTileByIndex(hitIndex2);
				int32 startStorageIndex = 0;

				if (isPawn(hitValue2)) {
					UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
					GetMovableIndicesPawn(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} else if (isRook(hitValue2)) {
					GetMovableIndicesRook(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} else if (isKnight(hitValue2)) {
					GetMovableIndicesKnight(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} else if (isBishop(hitValue2)) {
					GetMovableIndicesBishop(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} else if (isQueen(hitValue2)) {
					GetMovableIndicesQueen(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} else if (isKing(hitValue2)) {
					GetMovableIndicesKing(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				}
			}
			return;
		}
		// else:
		// change clickCount to 0;
		// updateTransform location of instance[index1] to location of [index2]
		// if hitIndex2 have piece on it (value != 0) -> update location of piece to graveyard +  set that ChessboardValue[index] == 0 && set ISMMapOn196 to -1
		// unhighlight selectedTile + movableTiles
		// TODO: check if the new position of this piece is checking the enemy
		// TODO: check if the new position of this piece is exposing our King to the enemy (being checked)
		clickCount = 0;
		if (ChessBoardValues[hitIndex2] != 0) 
		{
			Pieces->ISM_Pawn->UpdateInstanceTransform(ISMIndexMapOn196Board[hitIndex2], FTransform(FVector(0.f,0.f,0.f)), true, true);
		}
		FVector newLocation = IndexToLocation(hitIndex2);
		Pieces->ISM_Pawn->UpdateInstanceTransform(ISMIndexMapOn196Board[hitIndex1],FTransform(newLocation), true, true);
		UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is at location: %s!"), *newLocation.ToString());
		for (int i = 0; i < ActiveIndex; i++) 
		{
			UnhighlightTileByIndex(i, false);
		}
		UnhighlightTileByIndex(hitIndex1,true);

		// exchange index in the end
		exchange(hitIndex1, hitIndex2, ChessBoardValues);
		exchange(hitIndex1, hitIndex2, ISMIndexMapOn196Board);
	}
}

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
	const int32 indexToHighlight = XYToIndex(XYIndex.X, XYIndex.Y);
	UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

void ACustomPlayerController::HighlightSelectedTileByIndex(const int32& indexToHighlight)
{
	if (!Board)
		UE_LOG(LogTemp, Warning, TEXT("The board is NULL"));
	UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

// TODO: THIS IS NOT FINISHED, the color numeric values are just place holders
void ACustomPlayerController::HighlightMovableIndices(const int32& startMovableIndex = 0)	// by default we pass 0 if no value is passed in
{
	// highlight all movable indices
	int32 i = startMovableIndex;
	int32 currentIndexValue;
	for (/*  */; i < ActiveIndex - 1; i++)
	{
		currentIndexValue = MovableIndices[i];
		if (currentIndexValue == 0)
			// Normal Highlight (blank tile)
			Board->InstancedStaticMeshComponent->SetCustomDataValue(currentIndexValue, 0, 1.f, false);
		else
			// highlight RED (not blank, contains enemy piece and thus Eatable)
			Board->InstancedStaticMeshComponent->SetCustomDataValue(currentIndexValue, 0, 1.f, false);
	}

	// we want to MarkRenderDirty only at last of loop (bcoz it's costly), thus separate
	// this i is the 'failed condition' i, which means it has already incremented to activeIndex - 1 (the last index)
	currentIndexValue = MovableIndices[i];
	UE_LOG(LogTemp, Warning, TEXT("last movable Index is: %d"), currentIndexValue);
	// same check logic
	if (ChessBoardValues[currentIndexValue] == 0)
		Board->InstancedStaticMeshComponent->SetCustomDataValue(currentIndexValue, 0, 1.f, true);
	else
		Board->InstancedStaticMeshComponent->SetCustomDataValue(currentIndexValue, 0, 1.f, true);
}

void ACustomPlayerController::UnhighlightTileByIndex(const int32& index, const bool& bMarkRenderStateDirty = false) 
{// change board material back to its original state
 // TODO: do a correct implementation of this after change the M_material in Unreal Editor
	Board->InstancedStaticMeshComponent->SetCustomDataValue(index, 0, 0.f, bMarkRenderStateDirty);
}

FVector2D ACustomPlayerController::LocationToXY(const FVector& hitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Location2Index: Receive Location: %s"), *hitLocation.ToString());
	return FVector2D(hitLocation.X, hitLocation.Y);
}

int32 ACustomPlayerController::XYToIndex(const int32& XValue, const int32& YValue)
{
	return XValue / 400 * 14 + YValue / 400;
}

FVector ACustomPlayerController::IndexToLocation(const int32& index) 
{
	int32 X = index / 14 * 400 + 200;
	int32 Y = index % 14 * 400 + 200;
	return FVector(X,Y,0.f);
}

bool ACustomPlayerController::checkMate(const int32& playerIndex) 
{

	const int32& kingBottomPosition = ChessBoardValues.Find(5);
	const int32& kingTopPosition = ChessBoardValues.Find(15);
	const int32& kingLeftPosition = ChessBoardValues.Find(-5);
	const int32& kingRightPosition = ChessBoardValues.Find(-15);

	if (playerIndex == TOP || playerIndex == BOTTOM)
	{
		if (MovableIndices.Contains(kingLeftPosition)) 
		{
			UE_LOG(LogTemp,Warning,TEXT("player %d check mate player %d"), playerIndex, LEFT);
			// highlight here until resolve? Or set a timer for checkMate highlight?
		}
		if (MovableIndices.Contains(kingRightPosition))  
		{
			UE_LOG(LogTemp,Warning,TEXT("player %d check mate player %d"), playerIndex, RIGHT);
			// highlight here until resolve? Or set a timer for checkMate highlight?
		}
		return true;
	} 
	if (playerIndex == LEFT || playerIndex == RIGHT)  
	{
		if (MovableIndices.Contains(kingBottomPosition)) 
		{
			UE_LOG(LogTemp,Warning,TEXT("player %d check mate player %d"), playerIndex, BOTTOM);
			return true;
		}
		if (MovableIndices.Contains(kingTopPosition))  
		{
			UE_LOG(LogTemp,Warning,TEXT("player %d check mate player %d"), playerIndex, TOP);
			return true;
		}
	}
	return false;
}

void ACustomPlayerController::exchange(const int32& index1, const int32& index2, TArray<int32> array)
{
	int32 tempValue = array[index1];
	array[index1] = array[index2];
	array[index2] = tempValue;
}

int32 ACustomPlayerController::LocationToIndex(const FVector& hitLocation)
{
	return (int32) hitLocation.X / 400 * 14 + (int32) hitLocation.Y / 400;
}

/*? TODO: FINISH ALL MovableIndices FUNCTIONS, The implementation for POSITIVE AND NEGATIVE TEAMS are just placeholders, until you get the PlayerController */

void ACustomPlayerController::GetMovableIndicesPawn(int32& startStorageIndex, const FVector& clickLocation)
{
	const int32 currentIndex = LocationToIndex(clickLocation);
	const int32 instanceIndexInInstancedArray = ISMIndexMapOn196Board[currentIndex];

	ActiveIndex = startStorageIndex;

	// general cases: always able to highlight these tiles (if satisfied)
	// TEMPORARY CODE FOR NEGATIVE TEAM (TEAM LEFT AND RIGHT)
	/*{
		if (currentIndex + 1 < threshold)
			MovableIndices[ActiveIndex++] = currentIndex + 1;
		// since we are doing demo for negative team, if value[position] > 0 then it's enemy
		if (ChessBoardValues[currentIndex - 13] > 0 && ChessBoardValues[currentIndex - 13] != 7)    	// might use value sign like 'negative' or 'positive' to detect team against current controller
		{
			MovableIndices[ActiveIndex++] = currentIndex - 13;
		}
		if (ChessBoardValues[currentIndex + 15] > 0 && ChessBoardValues[currentIndex + 15] != 7)
		{
			MovableIndices[ActiveIndex++] = currentIndex + 15;
		}
	} */


	// for bottom team
	{
		if (currentIndex + 14 < 196)
			MovableIndices[ActiveIndex++] = currentIndex + 14;
		// since we are doing demo for negative team, if value[position] > 0 then it's enemy
		if (ChessBoardValues[currentIndex - 13] < 0)    	// might use value sign like 'negative' or 'positive' to detect team against current controller
		{
			MovableIndices[ActiveIndex++] = currentIndex - 13;
		}
		if (ChessBoardValues[currentIndex + 15] < 0)
		{
			MovableIndices[ActiveIndex++] = currentIndex + 15;
		}
		if (ChessBoardValues[currentIndex + 13] < 0)
		{
			MovableIndices[ActiveIndex++] = currentIndex + 13;
		}
		if (ChessBoardValues[currentIndex - 15] < 0)
		{
			MovableIndices[ActiveIndex++] = currentIndex - 15;
		}
	}

	int32 dataIndexInInstancedDataArray = instanceIndexInInstancedArray * 4 - 1;
	UE_LOG(LogTemp,Warning,TEXT("data instance is at %d"), dataIndexInInstancedDataArray);

	// there's an explicit casting from float to bool here
	bool bFirstMove = Pieces->ISM_Pawn->PerInstanceSMCustomData[dataIndexInInstancedDataArray];

	UE_LOG(LogTemp,Warning,TEXT("Pawn At %d, with data at %d in Instanced array, have value: %d"), instanceIndexInInstancedArray, dataIndexInInstancedDataArray, bFirstMove);

	/* FIRST MOVE SPECIALS */
	// get the 4th data Value of the instance data array (have to multiply instanceIndex * Num of Custom Data - 1 (since index mark from '0'))
	// Pieces having 4 data are: Rook, King, Pawn
	if (bFirstMove) // first move
	{
		UE_LOG(LogTemp,Warning,TEXT("First move of 3 is triggered!"));
		if (ChessBoardValues[currentIndex + 28] == 0)
		{
			UE_LOG(LogTemp,Warning,TEXT("Move 3 is doable!"));
			MovableIndices[ActiveIndex++] = currentIndex + 28;
		}
		if (currentIndex == 17) 
		{
			// en passant
			if (ChessBoardValues[45] < 0)
			{
				MovableIndices[ActiveIndex++] = 44;
				// TODO: we use this en passant in OnLeftMouseClick Function to check whether to destroy the enemy
				// TODO: Research if en passant can be done anywhere else
				bEnPassant = true;
			}
		}
		// since we only set the 4th data (irrelevant to rendering) thus we mark the renderState = false
		Pieces->ISM_Pawn->SetCustomDataValue(instanceIndexInInstancedArray, 3, 0.f, false);
	} 
}

void ACustomPlayerController::GetMovableIndicesRook(int32& startStorageIndex, const FVector& clickLocation)
{
/* 
	 Moving Logic as follow:
	 + VERTICALLY: going up 1 row: = current Index +14 or Down 1 row: = currentIndex - 14
	 + HORIZONTALLY:
	   - Horizontal left threshold = last index of previous row 
	   - Horizontal right threshold = first index of next row
*/
	const int32 currentPieceIndex = LocationToIndex(clickLocation);
	const int32 currentX = LocationToXY(clickLocation).X;
	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook current Index: %d"), currentPieceIndex);

	const int32 leftThreshold = currentX / 400 * 14;
	const int32 rightThreshold = leftThreshold + 14 - 1;
	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook left threshold: %d"), leftThreshold);
	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook right threshold: %d"), rightThreshold);

	// assign all with the current index of the chess piece
	int32 goLeft  = currentPieceIndex - 1, 
		  goRight = currentPieceIndex + 1, 
		  goUp    = currentPieceIndex + 14, 
		  goDown  = currentPieceIndex - 14;
	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook Go Left: %d"), currentPieceIndex);

	ActiveIndex = startStorageIndex;
	// CALCULATE HORIZONTAL MovableIndices OF TEAMs (base on whether current Player IS TEAM NEGATIVE or POSITIVE) 
	while (goLeft >= leftThreshold)
	{
		if (ChessBoardValues[goLeft] == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("goLeft: Index to Add: %d"), goLeft);
			if (ChessBoardValues[goLeft] == 7) break;
			MovableIndices[ActiveIndex++] = goLeft;
			goLeft--;
		}
		// TODO: FINISH THE COMMENTED-OUT IMPLEMENTATION for NEGATIVE-value AND POSITIVE-values TEAMS for all of the 'else' statements
		else 
		{
/* 
			if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				// if reach one of our side then break instantly
				if (ChessBoardValues[goLeft] > 0) break;
				// if reach enemy side then take that 1 index into account then break instantly
				else
				{
					MovableIndices[activeIndex++] = goLeft;
					break;
				}
			}
			else    // for team NEGATIVE, all below similar to Positive Logic
			{
				if (ChessBoardValues[goLeft] < 0) break;
				else
				{
					MovableIndices[activeIndex++] = goLeft;
					break;
				}
			} 
*/
			break; // this break is just for temporary run without error, not the right logic
		}
	}
	while (goRight <= rightThreshold)
	{
		if (ChessBoardValues[goRight] == 0)
		{
			if (ChessBoardValues[goRight] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goRight: Index to Add: %d"), goRight);
			MovableIndices[ActiveIndex++] = goRight;
			goRight++;
		}
		else
		{
/* 
			if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				if (ChessBoardValues[goLeft] > 0) break;
				else
				{
					MovableIndices[activeIndex++] = goRight;
					break;
				}
			}
			else    // for team NEGATIVE
			{
				if (ChessBoardValues[goLeft] < 0) break;
				else
				{
					MovableIndices[activeIndex++] = goRight;
					break;
				}
			} 
*/
			break;
		}
	}
	while (goUp <= 195)
	{
		if (ChessBoardValues[goUp] >= 0)
		{
			if (ChessBoardValues[goUp] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goUp: Index to Add: %d"), goUp);
			MovableIndices[ActiveIndex++] = goUp;
			goUp += 14;
		}
		else
		{
/*
			if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				if (ChessBoardValues[goLeft] > 0) break;
				else
				{
					MovableIndices[activeIndex++] = goUp;
					break;
				}
			}
			else    // for team NEGATIVE
			{
				if (ChessBoardValues[goLeft] < 0) break;
				else
				{
					MovableIndices[activeIndex++] = goUp;
					break;
			}
		} 
*/
			break;
		}
	}
	while (goDown >= 0)
	{
		if (ChessBoardValues[goDown] >= 0)
		{
			if (ChessBoardValues[goDown] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goDown: Index to Add: %d"), goDown);
			MovableIndices[ActiveIndex++] = goDown;
			goDown -= 14;
		}
		else
		{
/* 
			if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				if (ChessBoardValues[goLeft] > 0) break;
				else
				{
					MovableIndices[activeIndex++] = goDown;
					break;
				}
			}
			else    // for team NEGATIVE
			{
				if (ChessBoardValues[goLeft] < 0) break;
				else
				{
					MovableIndices[activeIndex++] = goDown;
					break;
				}
			} 
*/
			break;
		}
	}
}

void ACustomPlayerController::GetMovableIndicesKnight(int32& startStorageIndex, const FVector& clickLocation)
{
	const int32 currentIndex = LocationToIndex(clickLocation);

	int32 standLUpRight = currentIndex + 29,
		  lieLUpRight = currentIndex + 16,
		  standLUpLeft = currentIndex + 27,
		  lieLUpLeft = currentIndex + 12,
		  standLDownRight = currentIndex - 27,
		  lieLDownRight = currentIndex - 12,
		  standLDownLeft = currentIndex - 29,
		  lieLDownLeft = currentIndex - 16;

	ActiveIndex = startStorageIndex;

	if (ChessBoardValues[standLUpRight] != 7) MovableIndices[ActiveIndex++] = standLUpRight;
	if (ChessBoardValues[standLUpLeft] != 7) MovableIndices[ActiveIndex++] = standLUpLeft;
	if (ChessBoardValues[lieLUpRight] != 7) MovableIndices[ActiveIndex++] = lieLUpRight;
	if (ChessBoardValues[lieLUpLeft] != 7) MovableIndices[ActiveIndex++] = lieLUpLeft;
	if (ChessBoardValues[standLDownRight] != 7) MovableIndices[ActiveIndex++] = standLDownRight;
	if (ChessBoardValues[standLDownLeft] != 7) MovableIndices[ActiveIndex++] = standLDownLeft;
	if (ChessBoardValues[lieLDownRight] != 7) MovableIndices[ActiveIndex++] = lieLDownRight;
	if (ChessBoardValues[lieLDownLeft] != 7) MovableIndices[ActiveIndex++] = lieLDownLeft;

	UE_LOG(LogTemp, Warning, TEXT("standLUpRight: Index to Add: %d"), standLUpRight);
	UE_LOG(LogTemp, Warning, TEXT("standLUpLeft: Index to Add: %d"), standLUpLeft);
	UE_LOG(LogTemp, Warning, TEXT("lieLUpRight: Index to Add: %d"), lieLUpRight);
	UE_LOG(LogTemp, Warning, TEXT("lieLUpLeft: Index to Add: %d"), lieLUpLeft);
	UE_LOG(LogTemp, Warning, TEXT("standLDownRight: Index to Add: %d"), standLDownRight);
	UE_LOG(LogTemp, Warning, TEXT("standLDownLeft: Index to Add: %d"), standLDownLeft);
	UE_LOG(LogTemp, Warning, TEXT("lieLDownRight: Index to Add: %d"), lieLDownRight);
	UE_LOG(LogTemp, Warning, TEXT("lieLDownLeft: Index to Add: %d"), lieLDownLeft);
}

void ACustomPlayerController::GetMovableIndicesBishop(int32& activeIndex, const FVector& clickLocation)
{
	const int32 currentIndex = LocationToIndex(clickLocation);

	int32 goUpLeft    = currentIndex + 14 - 1,
		  goUpRight   = currentIndex + 14 + 1,
	  	  goDownLeft  = currentIndex - 14 - 1,
		  goDownRight = currentIndex - 14 + 1;

	ActiveIndex = activeIndex;

	// for POSTIVE TEAM
	while (goUpLeft <= 195)
	{
		if (ChessBoardValues[goUpLeft] == 0)
		{
			// reach unplayable pRightosition (which will break this plus rule)
			if (ChessBoardValues[goUpLeft] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goUpLeft: Index to Add: %d"), goUpLeft);
			// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
			MovableIndices[ActiveIndex++] = goUpLeft;
			goUpLeft = goUpLeft + 14 - 1;
		}
		else
		{
/*
			if (team1 turn)
			{
				if (ChessBoardValues[goUpLeft] > 0) break;
				else if (ChessBoardValues[goUpLeft] < 0)
				{
					MovableIndices[activeIndex++] = goUpLeft;
					break;
				}
				else break;
			}
			else
			{
				opposite;
			}
*/
		break; // this break is just for temporary run without error
		}
	}

	while (goUpRight <= 195)
	{
		if (ChessBoardValues[goUpRight] == 0)
		{
			if (ChessBoardValues[goUpRight] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goUpRight: Index to Add: %d"), goUpRight);
			// go up and shift left 1 unit relatively to current position (which already shift left 1 unit compared to the positon before it - meaning that total deviation compared to original position already increases)
			MovableIndices[ActiveIndex++] = goUpRight;
			goUpRight = goUpRight + 14 + 1;
		}
		else
		{
/*
			if (team1 turn)
			{
				if (ChessBoardValues[goUpLeft] > 0) break;
				else if (ChessBoardValues[goUpLeft] < 0)
				{
					MovableIndices[activeIndex++] = goUpLeft;
					break;
				}
				else break;
			}
			else
			{
				opposite;
			}
*/
		break; // this break is just for temporary run without error
		}
	}

	while (goDownLeft >= 0)
	{
		if (ChessBoardValues[goDownLeft] == 0)
		{
			if (ChessBoardValues[goDownLeft] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goDownLeft: Index to Add: %d"), goDownLeft);
			// shift left 1 unit relatively to current position (which already shift left 1 unit compared to positon before it)
			MovableIndices[ActiveIndex++] = goDownLeft;
			goDownLeft = goDownLeft - 14 - 1;
		}
		else
		{
/*
			if (team1 turn)
			{
				if (ChessBoardValues[goUpLeft] > 0) break;
				else if (ChessBoardValues[goUpLeft] < 0)
				{
					MovableIndices[activeIndex++] = goUpLeft;
					break;
				}
				else break;
			}
			else
			{
				opposite;
			}
*/
		break; // this break is just for temporary run without error
		}
	}

	while (goDownRight >= 0)
	{
		if (ChessBoardValues[goDownRight] == 0)
		{
			if (ChessBoardValues[goDownRight] == 7) break;
			UE_LOG(LogTemp, Warning, TEXT("goDownRight: Index to Add: %d"), goDownRight);
			// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
			MovableIndices[ActiveIndex++] = goDownRight;
			goDownRight = goDownRight - 14 + 1;
		}
		else
		{
/*
			if (team1 turn)
			{
				if (ChessBoardValues[goUpLeft] > 0) break;
				else if (ChessBoardValues[goUpLeft] < 0)
				{
					MovableIndices[activeIndex++] = goUpLeft;
					break;
				}
				else break;
			}
			else
			{
				opposite;
			}
*/
		break; // this break is just for temporary run without error
		}
	}
}

void ACustomPlayerController::GetMovableIndicesQueen(int32& startStorageIndex, const FVector& clickLocation)
{
	GetMovableIndicesBishop(startStorageIndex, clickLocation);
	GetMovableIndicesRook(ActiveIndex, clickLocation);
}

void ACustomPlayerController::GetMovableIndicesKing(int32& startStorageIndex, const FVector& clickLocation)
{
	const int32 currentIndex = LocationToIndex(clickLocation);

	// VERTICAL AND HORIZONTAL MOVES
	const int32 kingUp = currentIndex + 14,
		kingUpLeft = kingUp - 1,
		kingUpRight = kingUp + 1,
		kingLeft = currentIndex - 1,
		kingRight = currentIndex + 1,
		kingDown = currentIndex - 14,
		kingDownLeft = kingDown - 1,
		kingDownRight = kingDown + 1;

	// DIAGONAL MOVES

	ActiveIndex = startStorageIndex;
	if (ChessBoardValues[kingUpLeft] != 7) MovableIndices[ActiveIndex++] = kingUpLeft;
	if (ChessBoardValues[kingUp] != 7) MovableIndices[ActiveIndex++] = kingUp;
	if (ChessBoardValues[kingUpRight] != 7) MovableIndices[ActiveIndex++] = kingUpRight;
	if (ChessBoardValues[kingLeft] != 7) MovableIndices[ActiveIndex++] = kingLeft;
	if (ChessBoardValues[kingRight] != 7) MovableIndices[ActiveIndex++] = kingRight;
	if (ChessBoardValues[kingDownLeft] != 7) MovableIndices[ActiveIndex++] = kingDownLeft;
	if (ChessBoardValues[kingDown] != 7) MovableIndices[ActiveIndex++] = kingDown;
	if (ChessBoardValues[kingDownRight] != 7) MovableIndices[ActiveIndex++] = kingDownRight;

	// DEBUG purpose
	for (int i = 0; i < ActiveIndex; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("King MovableIndices are: %d"), MovableIndices[i]);
	}
}

// VALIDATION OF CLICK 1
bool ACustomPlayerController::isClick1P1IndexValid(const int32& hitIndex) 
{
	if (ChessBoardValues[hitIndex] == 0) return false;
	if (ChessBoardValues[hitIndex] < 1 || ChessBoardValues[hitIndex] > 6) return false;
	return isIndexValid(hitIndex);
}

bool ACustomPlayerController::isClick1P2IndexValid(const int32& hitIndex) 
{
	if (ChessBoardValues[hitIndex] == 0) return false;
	if (ChessBoardValues[hitIndex] < -6 || ChessBoardValues[hitIndex] > -1) return false;
	return isIndexValid(hitIndex);
}

bool ACustomPlayerController::isClick1P3IndexValid(const int32& hitIndex) 
{
	if (ChessBoardValues[hitIndex] == 0) return false;
	if (ChessBoardValues[hitIndex] < 11 || ChessBoardValues[hitIndex] > 16) return false;
	return isIndexValid(hitIndex);
}

bool ACustomPlayerController::isClick1P4IndexValid(const int32& hitIndex) 
{
	if (ChessBoardValues[hitIndex] == 0) return false;
	if (ChessBoardValues[hitIndex] < -16 || ChessBoardValues[hitIndex] > -11) return false;
	return isIndexValid(hitIndex);
}

// VALIDATION OF CLICK 2
// Because after a Valid Click 1 we've already got MovableIndices of that specific player and piece -> just need 1 check for all kinds of player
bool ACustomPlayerController::isClick2MoveIndexValid(const int32& hitIndex) 
{// see if the MovableIndices contains the hitIndex
	return MovableIndices.Contains(hitIndex);
}

bool ACustomPlayerController::isIndexValid(const int32& index) 
{
	return index > 0 && index < 196 && ChessBoardValues[index] != 7;
}

// Checking the type of piece
bool ACustomPlayerController::isPawn(const int32& pieceValue) 
{
	return abs(pieceValue)  == 1 || abs(pieceValue) == 11;
}


bool ACustomPlayerController::isRook(const int32& pieceValue) 
{
	return abs(pieceValue)  == 2 || abs(pieceValue) == 12;
}


bool ACustomPlayerController::isKnight(const int32& pieceValue) 
{
	return abs(pieceValue)  == 3 || abs(pieceValue) == 13;
}

bool ACustomPlayerController::isBishop(const int32& pieceValue) 
{
	return abs(pieceValue)  == 4 || abs(pieceValue) == 14;
}


bool ACustomPlayerController::isQueen(const int32& pieceValue) 
{
	return abs(pieceValue)  == 5 || abs(pieceValue) == 15;
}


bool ACustomPlayerController::isKing(const int32& pieceValue) 
{
	return abs(pieceValue)  == 6 || abs(pieceValue) == 16;
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Board = GetWorld()->SpawnActor<AChessBoard>();
	Pieces = GetWorld()->SpawnActor<AChessPieces>();


/* 

	The guy Spawn the Board as Separate Actor (one instance of type ChessBoard)
	Then Spawn the Whites and the Blacks

	For us we can also Spawn the ChessPiece as Separate Actor (one instance of typeChessPiece)

*/

}
