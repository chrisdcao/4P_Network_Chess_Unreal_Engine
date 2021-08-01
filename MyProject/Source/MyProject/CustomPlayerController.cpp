// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "ChessBoard.h"
#include "ChessPieces.h"

#define OUT

// Currently non-compilable as the functions are not completed yet
ACustomPlayerController::ACustomPlayerController() 
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;

	Board = nullptr;
	Pieces = nullptr;


	// init all the mechanics arrays and variables here
	// remember to swap the value of the mechanic array whenever you move
}

void ACustomPlayerController::SetupInputComponent()
{
	// Always call this.
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ACustomPlayerController::OnLeftMouseClick);
	InputComponent->BindAction("ShiftLeftMouseClick", IE_Pressed, this, &ACustomPlayerController::OnShiftLeftMouseClick);
}

void ACustomPlayerController::OnLeftMouseClick()
{
	SelectPiece();
}

void ACustomPlayerController::OnShiftLeftMouseClick()
{

}

// Select Piece = HighlightCurrentIndex + HighlightMovablesTiles
void ACustomPlayerController::SelectPiece()
{
	FHitResult TraceResult(ForceInit);
	// might have to change the collision Channel to Visible, because Our objects are in 1 mesh (thus cannot detect the different type of ECC for each object)
	GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, true, OUT TraceResult);

	FVector HitLocation = TraceResult.Location;

	UE_LOG(LogTemp, Warning, TEXT("Mouse hit at %s"), *HitLocation.ToString());

	if (isClickValid(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectPiece: Click is Valid"));
		HighlightTiles(HitLocation);
	}
}

// LOGIC: click -> location -> index -> valueAtIndex -> ChessPiece(int) -> getMovables(indices) -> Location -> transform -> move

// We will have materials at FIXED indices that serve the following function:
// [0]. Coloring Tile type1 (depends on Tile)
// [1]. Coloring Tile type2 (depends on Tile)
// [2]. Pure highlight (default = 0.f)
void ACustomPlayerController::HighlightTiles(const FVector& hitLocation)
{
	if (!Board)
		UE_LOG(LogTemp, Warning, TEXT("The board is NULL"));
	int32 indexToHighlight = LocationToIndex(hitLocation);
	UE_LOG(LogTemp, Warning, TEXT("Highlight Tile Index: %d"), indexToHighlight);
	Board->InstancedStaticMeshComponent->SetCustomDataValue(indexToHighlight, 0, 1.f, true);
}

int ACustomPlayerController::LocationToIndex(const FVector& hitLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Location2Index: Receive Location: %s"), *hitLocation.ToString());
	auto XValue = hitLocation.X;
	auto YValue = hitLocation.Y;
	int32 row = XValue/400;
	int32 col = YValue/400;

	return row * 14 + col;
}

bool ACustomPlayerController::isClickValid(const FVector& hitLocation)
{

	UE_LOG(LogTemp, Warning, TEXT("isClickValid(): Mouse hit at %s"), *hitLocation.ToString());
	if (hitLocation.X <= 5600 && hitLocation.X > 0 && hitLocation.Y <= 5600 && hitLocation.Y > 0) return true;
	return false;
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
