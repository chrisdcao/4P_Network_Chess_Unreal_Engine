// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "ChessBoard.h"
#include "ChessPieces.h"
#include <algorithm>

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
	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;

	offlineCount = 1;
	Board = nullptr;
	Pieces = nullptr;
	clickCount = 0;
	hitIndexOn196_1 = -1;
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

	// TODO: fix this back to starting from '0'
	// TODO: fix the function calculating data index in ISM to + 1 before multiply (for the table has changed back to starting from 0)
	Index196ToISMIndexPlusOne = {
		-1, -1, -1, 1, 1, 1, 1, 1, 2, 2, 2, -1, -1, -1,
	- 1, -1, -1, 1, 2, 3, 4, 5, 6, 7, 8, -1, -1, -1,
	- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	5, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, 7,
	5, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, 7,
	5, 19, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, 7,
	3, 20, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, 4,
	3, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 29, 4,
	6, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, 8,
	6, 23, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, 8,
	6, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, 8,
	- 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	- 1, -1, -1, 9, 10, 11, 12, 13, 14, 15, 16, -1, -1, -1,
	- 1, -1, -1, 3, 3, 3, 2, 2, 4, 4, 4, -1, -1, -1
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
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P1IndexValid(hitIndexOn196_1)) return;
		} 
/* 
		//if (player2) 
		{
			hitLocation = TraceResult.Location;
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P2IndexValid(hitIndexOn196_1)) return;
		} 
		//if (player3) 
		{
			hitLocation = TraceResult.Location;
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P3IndexValid(hitIndexOn196_1)) return;
		} 
		//if (player4) 
		{
			hitLocation = TraceResult.Location;
			hitIndexOn196_1 = LocationToIndex(hitLocation);
			if (!isClick1P4IndexValid(hitIndexOn196_1)) return;
		} 
*/
		clickCount = 1;
		const int32 hitValue1 = ChessBoardValues[hitIndexOn196_1];
		int32 startStorageIndex = 0;
		HighlightSelectedTileByIndex(hitIndexOn196_1);

		UE_LOG(LogTemp, Warning, TEXT("Hit Value is %d!"), ChessBoardValues[hitIndexOn196_1]);
		UE_LOG(LogTemp, Warning, TEXT("Hit Index is %d!"), hitIndexOn196_1);
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
	int32 hitIndexOn196_2;
	if (clickCount == 1) 
	{ 
		hitIndexOn196_2 = LocationToIndex(hitLocation);
		UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is triggered!"));
		UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is at index: %d!"), hitIndexOn196_2);
		// just added to return immediately if still click on the same place
		// WE CAN MERGE THIS TO THE BELOW IF WE WANT
		if (hitIndexOn196_2 == hitIndexOn196_1) return;
		// THERE ARE 2 POSSIBILITIES IF HITINDEXON196_2 IS NOT VALID
		if (!isClick2MoveIndexValid(hitIndexOn196_2)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is not valid at %d with value %d!"), hitIndexOn196_2, ChessBoardValues[hitIndexOn196_2]);
			for (int i = 0; i < ActiveIndex; i++)
				UE_LOG(LogTemp, Warning, TEXT("Movable indices at %d contains index: %d!"), i, MovableIndices[i]);
			// if it's the  player click again on one of his piece(which means click1 is valid), we cast it back to click1
			// 1.CLICKING ON ANOTHER PIECE OF OUR SIDE
			if (isClick1P1IndexValid(hitIndexOn196_2)) 
			{
				//DEBUG
				UE_LOG(LogTemp,Warning,TEXT("Cast back to click1 from click2"));
				// unhighlight all the past highlighted tiles
				for (int i = 0; i < ActiveIndex; i++) 
					UnhighlightTileByIndex(MovableIndices[i], false);
				UnhighlightTileByIndex(hitIndexOn196_1, true);
				// just assign hitIndex1 <- hitIndex2, as we re-select and see this as click1 again, waiting for click2
				hitIndexOn196_1 = hitIndexOn196_2;

				// recalculate and re-highlight all the stuffs just like click1
				const int32 hitValue2 = ChessBoardValues[hitIndexOn196_2];
				HighlightSelectedTileByIndex(hitIndexOn196_2);
				int32 startStorageIndex = 0;

				if (isPawn(hitValue2)) 
				{
					UE_LOG(LogTemp, Warning, TEXT("Pawn!"));
					GetMovableIndicesPawn(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} 
				else if (isRook(hitValue2)) 
				{
					GetMovableIndicesRook(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} 
				else if (isKnight(hitValue2)) 
				{
					GetMovableIndicesKnight(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} 
				else if (isBishop(hitValue2)) 
				{
					GetMovableIndicesBishop(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} 
				else if (isQueen(hitValue2)) 
				{
					GetMovableIndicesQueen(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				} 
				else if (isKing(hitValue2)) 
				{
					GetMovableIndicesKing(startStorageIndex, hitLocation);
					HighlightMovableIndices(0);
				}
			}
		}
		else
		{
			// since we have escaped the first condition, the following will be ...
			// else:
			// change clickCount to 0;
			// updateTransform location of instance[index1] to location of [index2]
			// if hitIndexOn196_2 have piece on it (value != 0) -> update location of piece to graveyard +  set that ChessboardValue[index] == 0 && set ISMMapOn196 to -1
			// unhighlight selectedTile + movableTiles
			// TODO: check if the new position of this piece is checking the enemy
			// TODO: check if the new position of this piece is exposing our King to the enemy (being checked)
			clickCount = 0;

			// TODO: if the chessBoardValues at the index we're about to move in != 0, move the piece to graveyard location
			if (ChessBoardValues[hitIndexOn196_2] != 0)  
				moveToGraveyard(ChessBoardValues[hitIndexOn196_2], hitIndexOn196_2);

			makeMove(hitIndexOn196_1, hitIndexOn196_2);
		}
	}
}


// TODO: so probably I still have to do a separate makeMove() function to correctly update the status of the Pawn or king firstMove
// TODO: or if this function (which highly likely) check the condition before executing then I can just based on the condition to disable?
// TODO: This exchange is having some problem. You might want to change to normal assignment
void ACustomPlayerController::makeMove(const int32& indexOn196_1, const int32& indexOn196_2) 
{
	const int32& pieceValue = ChessBoardValues[indexOn196_1];
	FVector newLocation = Index196ToLocation(indexOn196_2);
	UE_LOG(LogTemp, Warning, TEXT("Left mouse click 2 is at location: %s!"), *newLocation.ToString());
	// Logic here: if the piece is of 4-data type (king, rook, or pawn), then we might have to change the bFirstMove
	if (isPawn(pieceValue)) 
	{
		const int32& dataIndexInInstancedArray = Index196ToISMIndexPlusOne[indexOn196_1] * 4 - 1;
		///TODO: check the first move and change the status if still == true
		if (Pieces->ISM_Pawn->PerInstanceSMCustomData[dataIndexInInstancedArray])
			Pieces->ISM_Pawn->SetCustomDataValue(Index196ToISMIndexPlusOne[indexOn196_1]-1,3, 0.f);

		// update the location of the piece
		UE_LOG(LogTemp, Warning, TEXT("Index of piece about to move: %d!"), Index196ToISMIndexPlusOne[indexOn196_1] - 1);
		Pieces->ISM_Pawn->UpdateInstanceTransform(Index196ToISMIndexPlusOne[indexOn196_1]-1,FTransform(newLocation), true, true);
	}
	else if (isKing(pieceValue)) 
	{
		const int32& dataIndexInInstancedArray = Index196ToISMIndexPlusOne[indexOn196_1] * 4 - 1;
		///TODO: check the first move and change the status if still == true
		if (Pieces->ISM_King->PerInstanceSMCustomData[dataIndexInInstancedArray])
			Pieces->ISM_King->SetCustomDataValue(Index196ToISMIndexPlusOne[indexOn196_1]-1,3, 0.f);

		// update the location of the piece
		Pieces->ISM_King->UpdateInstanceTransform(Index196ToISMIndexPlusOne[indexOn196_1]-1,FTransform(newLocation), true, true);
	}
	else if (isRook(pieceValue)) 
	{
		const int32& dataIndexInInstancedArray = Index196ToISMIndexPlusOne[indexOn196_1] * 4 - 1;
		///TODO: check the first move and change the status if still == true
		if (Pieces->ISM_Rook->PerInstanceSMCustomData[dataIndexInInstancedArray])
			Pieces->ISM_Rook->SetCustomDataValue(Index196ToISMIndexPlusOne[indexOn196_1]-1,3, 0.f);

		Pieces->ISM_Rook->UpdateInstanceTransform(Index196ToISMIndexPlusOne[indexOn196_1]-1,FTransform(newLocation), true, true);
	}
	else if (isBishop(pieceValue))
		Pieces->ISM_Bishop->UpdateInstanceTransform(Index196ToISMIndexPlusOne[indexOn196_1]-1,FTransform(newLocation), true, true);
	else if (isQueen(pieceValue))
		Pieces->ISM_Queen->UpdateInstanceTransform(Index196ToISMIndexPlusOne[indexOn196_1]-1,FTransform(newLocation), true, true);
	else if (isKnight(pieceValue))
		Pieces->ISM_Knight->UpdateInstanceTransform(Index196ToISMIndexPlusOne[indexOn196_1]-1,FTransform(newLocation), true, true);

	// unhighlight all the suggested Tiles
	for (int i = 0; i < ActiveIndex; i++) 
		UnhighlightTileByIndex(MovableIndices[i], false);
	UnhighlightTileByIndex(indexOn196_1,true);

	// exchange index in the end
	// FOR enemies, we would like to change the index at that to '0' first
	// TODO: check if exchange() has already done that for us
	exchange(indexOn196_1, indexOn196_2, ChessBoardValues);
	exchange(indexOn196_1, indexOn196_2, Index196ToISMIndexPlusOne);
}

void ACustomPlayerController::moveToGraveyard(const int32& pieceValue, const int32& hitIndexOn196_2) 
{ // TODO: CHECK WHAT THE PIECE TYPE WE ARE GOING TO UPDATE TO GRAVEYARD
	const int32 ISMTo196Index = Index196ToISMIndexPlusOne[hitIndexOn196_2];

	// if there's more than one pawn then we just destroy the component and add to the 'x' beside the current pawn in graveyard
	if (isPawn(pieceValue)) 
		Pieces->ISM_Pawn->UpdateInstanceTransform(ISMTo196Index, FTransform(FVector(0.f,0.f,0.f)), true, true);
	if (isRook(pieceValue)) 
		Pieces->ISM_Rook->UpdateInstanceTransform(ISMTo196Index, FTransform(FVector(0.f,0.f,0.f)), true, true);
	if (isKnight(pieceValue)) 
		Pieces->ISM_Knight->UpdateInstanceTransform(ISMTo196Index, FTransform(FVector(0.f,0.f,0.f)), true, true);
	if (isBishop(pieceValue)) 
		Pieces->ISM_Bishop->UpdateInstanceTransform(ISMTo196Index, FTransform(FVector(0.f,0.f,0.f)), true, true);
	if (isQueen(pieceValue)) 
		Pieces->ISM_Queen->UpdateInstanceTransform(ISMTo196Index, FTransform(FVector(0.f,0.f,0.f)), true, true);
	if (isKing(pieceValue)) 
		Pieces->ISM_King->UpdateInstanceTransform(ISMTo196Index, FTransform(FVector(0.f,0.f,0.f)), true, true);
	
	// after updating to the graveyard, we change the value at that index to 0 and at that index in ISM to -1
	ChessBoardValues[hitIndexOn196_2] = 0;
	Index196ToISMIndexPlusOne[hitIndexOn196_2] = -1;
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
	if (ActiveIndex != 0)
	{
		// highlight all movable indices
		int32 i = startMovableIndex;
		int32 currentIndexValue;
		UE_LOG(LogTemp, Warning, TEXT("Active index at HMI: %d"), ActiveIndex);
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

FVector ACustomPlayerController::Index196ToLocation(const int32& index) 
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

void ACustomPlayerController::exchange(const int32& index1, const int32& index2, TArray<int32>& array)
{
	int32 tempValue = array[index1];
	array[index1] = array[index2];
	array[index2] = tempValue;
}

int32 ACustomPlayerController::LocationToIndex(const FVector& hitLocation)
{
	return (int32) hitLocation.X / 400 * 14 + (int32) hitLocation.Y / 400;
}

/* TODO: FINISH ALL MovableIndices FUNCTIONS, The implementation for POSITIVE AND NEGATIVE TEAMS are just placeholders, until you get the PlayerController */
/* TODO: Lam function NhapThanh() */

// TODO: CONSIDER EXTRACTING METHODS SEPARATELY FOR 4 SIDES, AS WE ARE GOING TO CHECK CONDITION FROM THE LEFT-CLICK ABOVE and MIGHT BE REPEATED IF WE CHECK THEM AGAIN HERE
void ACustomPlayerController::GetMovableIndicesPawn(int32& startStorageIndex, const FVector& clickLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("GetMovablesPawn is triggered!"));
	const int32 currentIndex = LocationToIndex(clickLocation);
	const int32 instanceIndexInInstancedArray = Index196ToISMIndexPlusOne[currentIndex];

	ActiveIndex = startStorageIndex;

	// TODO: for each team the Pawn will be moving in different axes, make sure to do them after you've done networking
	// for bottom team
	if (offlineCount == 1)
	{
		// BOTH OF THESE FUNCTIONS ARE ALREADY CLAMP WITHIN [0, 195]
		// TODO: Check if the piece is in range of player's playable pieces (not at this function but AT THE FUNCTION OnLeftMouseClick())
		/* MOVE TO MOVE MOVE: STRAIGHT FORWARD */
		if (currentIndex + 14 < 196)
			if (ChessBoardValues[currentIndex + 14] == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Value at tile ahead: %d"), ChessBoardValues[currentIndex + 14]);
				MovableIndices[ActiveIndex++] = currentIndex + 14;
			}

		/*  THE MOVE TO EAT (thus we don't consider == 0) */
		// since we are doing demo for positive team, if value[position] > 0 then it's enemy
		// since this is C++, we always have to do an check valid index first
		// TODO: check if we have to do a Tight Condition check (simulate if Pawn keeps going diagonal from the starting point all the way and see if it could reach invalid indices/places)
		// TODO:  YES WE DO, AS THE PAWN NEAR THE CORNER EASILY FALLS OFF THE THRESHOLD

		// left and right threshold are NON-STRICT
		// both these eats are on the same row as moving so we can use either of eatRight or eatLeft as argument
		const int16 eatRight = currentIndex + 15;
		const int16 eatLeft = currentIndex + 13;
		const int16 rightThreshold = RightThresholdOfIndex(eatRight);	
		const int16 leftThreshold = LeftThresholdOfIndex(eatRight);

		if (eatRight <= rightThreshold)
			// strict when comparing values (as 0 is the empty, 2 teams value are either completely negative or positive)
			if (ChessBoardValues[eatRight] < 0)
				MovableIndices[ActiveIndex++] = currentIndex + 15;
		if (eatLeft >= leftThreshold)
			if (ChessBoardValues[eatLeft + 13] < 0)
				MovableIndices[ActiveIndex++] = eatLeft;
	}

	// for left team - NEGATIVE
	if (offlineCount == 2)
	{
		const int32 moveRightThreshold = RightThresholdOfIndex(currentIndex);
		/* THE MOVE TO MOVE - STRAIGHT RIGHT */
		if (currentIndex + 1 < moveRightThreshold)
			if (ChessBoardValues[currentIndex + 1] == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Value at tile ahead: %d"), ChessBoardValues[currentIndex + 14]);
				MovableIndices[ActiveIndex++] = currentIndex + 1;
			}

		/*  THE MOVE TO EAT (thus we don't consider == 0) */
		const int32 eatUp = currentIndex + 15;
		const int32 eatDown = currentIndex - 13;
		const int16 eatUpRightThreshold = RightThresholdOfIndex(eatUp);
		const int16 eatDownRightThreshold = RightThresholdOfIndex(eatDown);

		if (eatUp <= eatUpRightThreshold && eatUp < 196)
			if (ChessBoardValues[eatUp] > 0 && ChessBoardValues[eatUp] != 7)
				MovableIndices[ActiveIndex++] = eatUp;
		if (eatDown <= eatDownRightThreshold && eatDown >= 0) // since there's no eating backwards so we don't have to compare with leftThreshold
			if (ChessBoardValues[eatDown] > 0 && ChessBoardValues[eatDown] != 7)
				MovableIndices[ActiveIndex++] = eatDown;
	}

	// for top team - POSITIVE
	if (offlineCount == 3)
	{
		/* THE MOVE TO MOVE */
		if (currentIndex - 14 < 196)
			if (ChessBoardValues[currentIndex - 14] == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Value at tile ahead: %d"), ChessBoardValues[currentIndex + 14]);
				MovableIndices[ActiveIndex++] = currentIndex - 14;
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
			if (ChessBoardValues[eatRight] < 0)
				MovableIndices[ActiveIndex++] = eatRight;
		if (eatLeft >= eatLeftThreshold)
			if (ChessBoardValues[eatLeft] < 0)
				MovableIndices[ActiveIndex++] = eatLeft;
	}

	// TODO : finish for right team
	// for right team - NEGATIVE
	if (offlineCount == 4)
	{
		const int16 moveLeftThreshold = LeftThresholdOfIndex(currentIndex);
		/* THE MOVE TO MOVE */
		if (currentIndex - 1 > moveLeftThreshold)
			if (ChessBoardValues[currentIndex - 1] == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Value at tile ahead: %d"), ChessBoardValues[currentIndex + 14]);
				MovableIndices[ActiveIndex++] = currentIndex - 1;
			}

		/*  THE MOVE TO EAT (thus we don't consider == 0) */
		// since we are doing demo for positive team, if value[position] > 0 then it's enemy
		// since this is C++, we always have to do an check valid index first
		// TODO: check if we have to do a Tight Condition check (simulate if Pawn keeps going diagonal from the starting point all the way and see if it could reach invalid indices/places)
		const int16 eatUp = currentIndex + 13;
		const int16 eatDown = currentIndex - 15;
		const int16 eatUpLeftThreshold = LeftThresholdOfIndex(eatUp);
		const int16 eatDownLeftThreshold = LeftThresholdOfIndex(eatDown);

		if (eatUp >= eatUpLeftThreshold && eatUpLeftThreshold < 196)
			if (ChessBoardValues[eatUp] > 0 && ChessBoardValues[eatUp] != 7)
				MovableIndices[ActiveIndex++] = eatUp;
		if (eatDown >= 0 && eatDown >= eatDownLeftThreshold)
			if (ChessBoardValues[eatDown] > 0 && ChessBoardValues[eatDown] != 7)
				MovableIndices[ActiveIndex++] = eatDown;
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
		// TODO: DIVIDE THIS INTO 4 CASES AS WELL. MIGHT DO AFTER EXTRACTING ALL 4 METHODS

		// FOR BOTTOM TEAM ONLY
		UE_LOG(LogTemp,Warning,TEXT("First move of 3 is triggered!"));
		if (currentIndex + 14 < 196)
			if (ChessBoardValues[currentIndex + 14] == 0)
				if (currentIndex + 28 < 196)
					if (ChessBoardValues[currentIndex + 28] == 0)
					{
						UE_LOG(LogTemp,Warning,TEXT("Move 3 is doable!"));
						MovableIndices[ActiveIndex++] = currentIndex + 28;
					}

		/* EN PASSANT */
		/* WAITING FOR VALIDATION ON LOGIC BEfore finish coding */
		// TODO: ensure that we don't have to check blocking condition for this
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
	} 

	// TODO: reassign turn at the bottom
	if (offlineCount == 1)
		offlineCount = 2;
	else if (offlineCount == 2)
		offlineCount = 3;
	else if (offlineCount == 3)
		offlineCount = 4;
	else if (offlineCount == 4)
		offlineCount = 1;

	for (int i = 0; i < ActiveIndex; i++)
		UE_LOG(LogTemp, Warning, TEXT("movable index: %d"), MovableIndices[i]);
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

	// the last index of the row below it
	const int32 LeftThreshold = currentX / 400 * 14;
	// the first index of the row above it
	const int32 RightThreshold = LeftThreshold + 14 - 1;	

	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook left threshold: %d"), LeftThreshold);
	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook right threshold: %d"), RightThreshold);

	// assign all with the current index of the chess piece
	int32 goLeft  = currentPieceIndex - 1, 
		  goRight = currentPieceIndex + 1, 
		  goUp    = currentPieceIndex + 14, 
		  goDown  = currentPieceIndex - 14;
	UE_LOG(LogTemp, Warning, TEXT("GetMovableIndicesRook Go Left: %d"), currentPieceIndex);

	ActiveIndex = startStorageIndex;
	// CALCULATE HORIZONTAL MovableIndices OF TEAMs (base on whether current Player IS TEAM NEGATIVE or POSITIVE) 
	while (goLeft >= LeftThreshold)
	{
		if (ChessBoardValues[goLeft] == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("goLeft: Index to Add: %d"), goLeft);
			MovableIndices[ActiveIndex++] = goLeft;
			goLeft--;
		}
		// TODO: FINISH THE COMMENTED-OUT IMPLEMENTATION for NEGATIVE-value AND POSITIVE-values TEAMS for all of the 'else' statements (FOR ALL THE TEAMS)
		else 
		{
			//if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				// if reach one of our side then break instantly
				if (ChessBoardValues[goLeft] > 0) break;
				// if reach enemy side then take that 1 index into account then break instantly
				else
				{
					MovableIndices[ActiveIndex++] = goLeft;
					break;
				}
			}
/*
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
	while (goRight <= RightThreshold)
	{
		if (ChessBoardValues[goRight] == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("goRight: Index to Add: %d"), goRight);
			MovableIndices[ActiveIndex++] = goRight;
			goRight++;
		}
		else
		{
			//if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				if (ChessBoardValues[goRight] > 0) break;
				else
				{
					MovableIndices[ActiveIndex++] = goRight;
					break;
				}
			}
/* 
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
		if (ChessBoardValues[goUp] == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("goUp: Index to Add: %d"), goUp);
			MovableIndices[ActiveIndex++] = goUp;
			goUp += 14;
		}
		else
		{
			//if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				if (ChessBoardValues[goUp] > 0) break;
				else
				{
					MovableIndices[ActiveIndex++] = goUp;
					break;
				}
			}
/*
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
		if (ChessBoardValues[goDown] == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("goDown: Index to Add: %d"), goDown);
			MovableIndices[ActiveIndex++] = goDown;
			goDown -= 14;
		}
		else
		{
			// if (player 1 turn || player 3 turn) // for team POSITIVE
			{
				if (ChessBoardValues[goDown] > 0) break;
				else
				{
					MovableIndices[ActiveIndex++] = goDown;
					break;
				}
			}
/* 
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

// TODO: KNIGHT ON THE RIGHT IS NOT GETTING THE MOVABLES PROPERLY (missing left movables)
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

	//TODO: have to compare these to the minimum of the board and maximum of the board
	// TODO: we can just put that right into the threshold calculator
	const int32 standUpLeftThreshold = LeftThresholdOfIndex(standLUpRight);
	const int32 standUpRightThreshold = RightThresholdOfIndex(standLUpRight);
	const int32 lieUpLeftThreshold = LeftThresholdOfIndex(lieLUpRight);
	const int32 lieUpRightThreshold = RightThresholdOfIndex(lieLUpRight);
	const int32 standDownLeftThreshold = LeftThresholdOfIndex(standLDownRight);
	const int32 standDownRightThreshold = RightThresholdOfIndex(standLDownLeft);
	const int32 lieDownRightThreshold = RightThresholdOfIndex(lieLDownLeft);
	const int32 lieDownLeftThreshold = LeftThresholdOfIndex(lieLDownRight);

	const int32 upThreshold = 195;
	const int32 downThreshold = 0;

	// TODO: you can think of merging the Left and RightThreshold (tight condition) with 195 and 0? 
	// check the validation of all 4 directions: up/down/left/right
	/* POSITIVE TEAM */
	// this is only for Positive teams
	// if current player is Positive Team
	if (standLUpRight <= standUpRightThreshold && standLUpRight <= upThreshold && standLUpRight >= downThreshold)
		if (ChessBoardValues[standLUpRight] <= 0) 
			MovableIndices[ActiveIndex++] = standLUpRight;

	UE_LOG(LogTemp, Warning, TEXT("standLUpleft Threshold is %d!"), standUpLeftThreshold);
	UE_LOG(LogTemp, Warning, TEXT("standLUpRight Threshold is %d!"), standUpRightThreshold);
	if (standLUpLeft >= standUpLeftThreshold && standLUpLeft <= upThreshold && standLUpLeft >= downThreshold)
		if (ChessBoardValues[standLUpLeft] <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("standLUpleft is added to the active index!"))
			MovableIndices[ActiveIndex++] = standLUpLeft;
		}

	if (lieLUpLeft >= lieUpLeftThreshold && lieLUpLeft <= upThreshold && lieLUpLeft >= downThreshold)
		if (ChessBoardValues[lieLUpLeft] <= 0) 
			MovableIndices[ActiveIndex++] = lieLUpLeft;

	if (lieLUpRight <= lieUpRightThreshold && lieLUpRight <= upThreshold && lieLUpRight >= downThreshold)
		if (ChessBoardValues[lieLUpRight] <= 0) 
			MovableIndices[ActiveIndex++] = lieLUpRight;

	if (standLDownLeft >= standDownLeftThreshold && standLDownLeft <= upThreshold && standLDownLeft >= downThreshold)
		if (ChessBoardValues[standLDownLeft] <= 0) 
			MovableIndices[ActiveIndex++] = standLDownLeft;

	if (standLDownRight <= standDownRightThreshold && standLDownRight <= upThreshold && standLDownRight >= downThreshold)
		if (ChessBoardValues[standLDownRight] <= 0) 
			MovableIndices[ActiveIndex++] = standLDownRight;

	if (lieLDownRight <= lieDownRightThreshold && lieLDownRight <= upThreshold && lieLDownRight >= downThreshold)
		if (ChessBoardValues[lieLDownRight] <= 0) 
			MovableIndices[ActiveIndex++] = lieLDownRight;

	if (lieLDownLeft >= lieDownLeftThreshold && lieLDownLeft <= upThreshold && lieLDownLeft >= downThreshold)
		if (ChessBoardValues[lieLDownLeft] <= 0) 
			MovableIndices[ActiveIndex++] = lieLDownLeft;
	
	UE_LOG(LogTemp, Warning, TEXT("StandLUpLeft: %d"), standLUpLeft);
	// active index has already got 1 more space extra compared to valued spaces
	for (int i = 0; i < ActiveIndex; i++)
		UE_LOG(LogTemp, Warning, TEXT("Indices for knights: %d"), MovableIndices[i]);

}

void ACustomPlayerController::GetMovableIndicesBishop(int32& activeIndex, const FVector& clickLocation)
{
	const int32 currentIndex = LocationToIndex(clickLocation);

	int32 goUpLeft    = currentIndex + 14 - 1,
		  goUpRight   = currentIndex + 14 + 1,
	  	  goDownLeft  = currentIndex - 14 - 1,
		  goDownRight = currentIndex - 14 + 1;

	ActiveIndex = activeIndex;

	// TODO: CREATE HORIZONTAL AND VERTICAL THRESHOLD FOR THE PIECE (TIGHT CONDITION)
	//In here we've already got the vertical threshold
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
				// TODO: we get one more index of the enemy team into movables and ONLY AFTERWARDS do we break
				else if (ChessBoardValues[goUpLeft] < 0)
				{
					MovableIndices[activeIndex++] = goUpLeft;
					break;
				}
				// else we just BREAK instantly
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
			// shift left 1 unit relatively to current position (which already shift left 1 unit compared to position before it)
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
/* THIS ONE NOT HAVING TIGHT THRESHOLD, MAKING SOME OF ITS INDICES INVALID */
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

// THESE THRESHOLD SHOULD BE COMPARED TO THE THRESHOLD OF THE WHOLE BOARD, SO THAT WE ARE NOT CONFUSE
// BOTH OF THESE FUNCTIONS ARE CLAMP WITHIN [0, 195]
int32 ACustomPlayerController::LeftThresholdOfIndex(const int32& index)
{
	return std::min(std::max(index / 14 * 14, 0), 195);
}

int32 ACustomPlayerController::RightThresholdOfIndex(const int32& index)
{
	return std::min(std::max(LeftThresholdOfIndex(index) + 14 - 1, 0), 195);
}

// VALIDATION OF CLICK 1
//this might not be as good as to change to storing one hit variable and compare the second click to the variable to see if we reset the click again 
bool ACustomPlayerController::isClick1P1IndexValid(const int32& hitIndexOn196_) 
{
	if (ChessBoardValues[hitIndexOn196_] == 0) return false;
	if (ChessBoardValues[hitIndexOn196_] < 1 || ChessBoardValues[hitIndexOn196_] > 6) return false;
	return isIndexValid(hitIndexOn196_);
}

bool ACustomPlayerController::isClick1P2IndexValid(const int32& hitIndexOn196_) 
{
	if (ChessBoardValues[hitIndexOn196_] == 0) return false;
	if (ChessBoardValues[hitIndexOn196_] < -6 || ChessBoardValues[hitIndexOn196_] > -1) return false;
	return isIndexValid(hitIndexOn196_);
}

bool ACustomPlayerController::isClick1P3IndexValid(const int32& hitIndexOn196_) 
{
	if (ChessBoardValues[hitIndexOn196_] == 0) return false;
	if (ChessBoardValues[hitIndexOn196_] < 11 || ChessBoardValues[hitIndexOn196_] > 16) return false;
	return isIndexValid(hitIndexOn196_);
}

bool ACustomPlayerController::isClick1P4IndexValid(const int32& hitIndexOn196_) 
{
	if (ChessBoardValues[hitIndexOn196_] == 0) return false;
	if (ChessBoardValues[hitIndexOn196_] < -16 || ChessBoardValues[hitIndexOn196_] > -11) return false;
	return isIndexValid(hitIndexOn196_);
}

// VALIDATION OF CLICK 2
// Because after a Valid Click 1 we've already got MovableIndices of that specific player and piece -> just need 1 check for all kinds of player
bool ACustomPlayerController::isClick2MoveIndexValid(const int32& hitIndexOn196) 
{
	UE_LOG(LogTemp, Warning, TEXT("hitINdexOn196: %d"), hitIndexOn196);
	UE_LOG(LogTemp, Warning, TEXT("ActiveIndex: %d"), ActiveIndex);
	// see if the MovableIndices contains the hitIndexOn196_
	if (ActiveIndex == 0) return false; // if no move available, we just mark as false so that it returns to waiting from click1
	// TODO: fix: see if movable indices WITHIN ACTIVE INDEX RANGE contains the value
	for (int i = 0; i < ActiveIndex; i++)
		if (hitIndexOn196 == MovableIndices[i]) 
			return true;
	return false;
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
