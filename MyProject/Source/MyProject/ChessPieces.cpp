// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessPieces.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// REDO THIS CHESSPIECES USING NORMAL INSTANCES (INSTEAD OF INSTANCED STATIC MESH)
// Sets default values
// TODO: REMOVE ROTATION OF ROOK,PAWN, QUEEN, probably KING (NOT NEEDED)
// TODO: TRY DOING THE CHESSPIECES AS AN ARRAY OF USTATICMESH INSTEAD OF ISM
AChessPieces::AChessPieces()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// General Params setup
	float x, y, z = 0.f;
	static ConstructorHelpers::FObjectFinder<UMaterial>PiecesMaterial(TEXT("Material'/Game/Materials/M_ChessPieces.M_ChessPieces'"));
	TArray<float> TeamColors;
	// Default: RED color
	TeamColors.Init(0.f, 3);
/* 
	4 DATA CHART 
	[0],[1],[2]: the first 3 are fixed as LERP COLOR CHART, because we have set the Node 'Per Instance Custom Data' in Material to be first 3 indices
	[3]:	    bFirstMove? default true (as it's start of the game)
*/
	TArray<float> FourDataPieces = { 0.f,0.f,0.f,1.f };	// default to RED color
/* 
		 LERP COLOR CHART
		 * RED color:	 0, 0, 0
		 * BLUE color:	 1, 0, 0
		 * GREEN color:	 0, 0, 1
		 * YELLOW color: 0, 1, 1
 */
	 /* PAWN */
	ISM_Pawn = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM_Pawn"));
	SetRootComponent(ISM_Pawn);
	// NOTE: the 4th value will be for the first move boolean, the first 3 are for the colors
	ISM_Pawn->NumCustomDataFloats = 4;
	ISM_Pawn->SetMobility(EComponentMobility::Movable);
	// ISM_Pawn->SetCollisionProfileName("BlockAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>PawnMeshAsset(TEXT("StaticMesh'/Game/ChessPieces/SM_Pawn.SM_Pawn'"));
	ISM_Pawn->SetStaticMesh(PawnMeshAsset.Object);
	ISM_Pawn->SetMaterial(0, PiecesMaterial.Object);

	/* ROOK */
	ISM_Rook = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM_Rook"));
	ISM_Rook->AttachToComponent(ISM_Pawn, FAttachmentTransformRules::KeepRelativeTransform);
	ISM_Rook->NumCustomDataFloats = 3;
	// ISM_Rook->SetMobility(EComponentMobility::Static);
	// ISM_Rook->SetCollisionProfileName("BlockAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>RookMeshAsset(TEXT("StaticMesh'/Game/ChessPieces/SM_Rook.SM_Rook'"));
	ISM_Rook->SetStaticMesh(RookMeshAsset.Object);
	ISM_Rook->SetMaterial(0, PiecesMaterial.Object);

	/* KNIGHT */
	ISM_Knight = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM_Knight"));
	ISM_Knight->AttachToComponent(ISM_Pawn, FAttachmentTransformRules::KeepRelativeTransform);
	ISM_Knight->NumCustomDataFloats = 3;
	// ISM_Knight->SetMobility(EComponentMobility::Static);
	// ISM_Knight->SetCollisionProfileName("BlockAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>KnightMeshAsset(TEXT("StaticMesh'/Game/ChessPieces/SM_Knight.SM_Knight'"));
	ISM_Knight->SetStaticMesh(KnightMeshAsset.Object);
	ISM_Knight->SetMaterial(0, PiecesMaterial.Object);

	/* BISHOP */
	ISM_Bishop = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM_Bishop"));
	ISM_Bishop->AttachToComponent(ISM_Pawn, FAttachmentTransformRules::KeepRelativeTransform);
	ISM_Bishop->NumCustomDataFloats = 3;
	// ISM_Bishop->SetMobility(EComponentMobility::Static);
	// ISM_Bishop->SetCollisionProfileName("BlockAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BishopMeshAsset(TEXT("StaticMesh'/Game/ChessPieces/SM_Bishop.SM_Bishop'"));
	ISM_Bishop->SetStaticMesh(BishopMeshAsset.Object);
	ISM_Bishop->SetMaterial(0, PiecesMaterial.Object);

	/* QUEEN */
	ISM_Queen = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM_Queen"));
	ISM_Queen->AttachToComponent(ISM_Pawn, FAttachmentTransformRules::KeepRelativeTransform);
	ISM_Queen->NumCustomDataFloats = 3;
	// ISM_Queen->SetMobility(EComponentMobility::Static);
	// ISM_Queen->SetCollisionProfileName("BlockAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>QueenMeshAsset(TEXT("StaticMesh'/Game/ChessPieces/SM_Queen.SM_Queen'"));
	ISM_Queen->SetStaticMesh(QueenMeshAsset.Object);
	ISM_Queen->SetMaterial(0, PiecesMaterial.Object);

	/* KING */
	ISM_King = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM_King"));
	ISM_King->AttachToComponent(ISM_Pawn, FAttachmentTransformRules::KeepRelativeTransform);
	ISM_King->NumCustomDataFloats = 3;
	// ISM_King->SetMobility(EComponentMobility::Static);
	// /ISM_King->SetCollisionProfileName("BlockAll");
	static ConstructorHelpers::FObjectFinder<UStaticMesh>KingMeshAsset(TEXT("StaticMesh'/Game/ChessPieces/SM_King.SM_King'"));
	ISM_King->SetStaticMesh(KingMeshAsset.Object);
	ISM_King->SetMaterial(0, PiecesMaterial.Object);


	// BOTTOM - Player
	/* PAWN */
	x = 600.f; y = 1400.f;
	for (int i = 0; i < 8; i++) {
		ISM_Pawn->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Pawn->SetCustomData(i, FourDataPieces, false);
		y += 400;
	}
	/* ROOK */
	x = 200.f; y = 1400.f;
	for (int i = 0; i < 2; i++) {
		ISM_Rook->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Rook->SetCustomData(i, FourDataPieces, false);
		y += 2800.f;
	}
	/* KNIGHT */
	x = 200.f; y = 1800.f;
	for (int i = 0; i < 2; i++) {
		ISM_Knight->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		y += 2000.f;
	}
	/* BISHOP */
	x = 200.f; y = 2200.f;
	for (int i = 0; i < 2; i++) {
		ISM_Bishop->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		y += 1200.f;
	}
	/* QUEEN */
	x = 200.f; y = 2600.f;
	ISM_Queen->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	/* KING */
	x = 200.f; y = 3000.f;
	ISM_King->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_King->SetCustomData(0, FourDataPieces, false);


	// TOP - Player 
	TeamColors[0] = 1.f;	// BLUE
	FourDataPieces[0] = 1.f;
	/* PAWN */
	x = 5000.f; y = 1400.f;
	for (int i = 8; i < 16; i++) {
		ISM_Pawn->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Pawn->SetCustomData(i, FourDataPieces, false);
		y += 400;
	}
	/* ROOK */
	x = 5400.f; y = 1400.f;
	for (int i = 2; i < 4; i++) {
		ISM_Rook->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Rook->SetCustomData(i, FourDataPieces, false);
		y += 2800.f;
	}
	/* KNIGHT */
	x = 5400.f; y = 1800.f;
	for (int i = 2; i < 4; i++) {
		ISM_Knight->AddInstanceWorldSpace(FTransform(FRotator(0.f,180.f,0.f),FVector(x, y, z),FVector(1.f)));
		ISM_Knight->SetCustomData(i, TeamColors, false);
		y += 2000.f;
	}
	/* BISHOP */
	x = 5400.f; y = 2200.f;
	for (int i = 2; i < 4; i++) {
		ISM_Bishop->AddInstanceWorldSpace(FTransform(FRotator(0.f,180.f,0.f),FVector(x, y, z),FVector(1.f)));
		ISM_Bishop->SetCustomData(i, TeamColors, false);
		y += 1200.f;
	}
	/* QUEEN */
	x = 5400.f; y = 3000.f;
	ISM_Queen->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_Queen->SetCustomData(1, TeamColors, false);
	/* KING */
	x = 5400.f; y = 2600.f;
	ISM_King->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_King->SetCustomData(1, FourDataPieces, false);


	// LEFT - Player 
	TeamColors[0] = 0.f;	// GREEN
	TeamColors[2] = 1.f;	// 
	FourDataPieces[0] = 0.f;	// GREEN
	FourDataPieces[2] = 1.f;	// 
	/* PAWN */
	x = 1400.f; y = 600.f;
	for (int i = 16; i < 24; i++) {
		ISM_Pawn->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Pawn->SetCustomData(i, FourDataPieces, false);
		x += 400;
	}
	/* ROOK */
	x = 1400.f; y = 200.f;
	for (int i = 4; i < 6; i++) {
		ISM_Rook->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Rook->SetCustomData(i, FourDataPieces, false);
		x += 2800.f;
	}
	/* KNIGHT */
	x = 1800.f; y = 200.f;
	for (int i = 4; i < 6; i++) {
		ISM_Knight->AddInstanceWorldSpace(FTransform(FRotator(0.f,90.f,0.f),FVector(x, y, z),FVector(1.f)));
		ISM_Knight->SetCustomData(i, TeamColors, false);
		x += 2000.f;
	}
	/* BISHOP */
	x = 2200.f; y = 200.f;
	for (int i = 4; i < 6; i++) {
		ISM_Bishop->AddInstanceWorldSpace(FTransform(FRotator(0.f,90.f,0.f),FVector(x, y, z),FVector(1.f)));
		ISM_Bishop->SetCustomData(i, TeamColors, false);
		x += 1200.f;
	}
	/* QUEEN */
	x = 2600.f; y = 200.f;
	ISM_Queen->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_Queen->SetCustomData(2, TeamColors, false);
	/* KING */
	x = 3000.f; y = 200.f;
	ISM_King->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_King->SetCustomData(2, FourDataPieces, false);


	// RIGHT - Player
	TeamColors[1] = 1.f;	// YELLOW
	FourDataPieces[1] = 1.f;	// YELLOW
	/* PAWN */
	x = 1400.f; y = 5000.f;
	for (int i = 24; i < 31; i++) {
		ISM_Pawn->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Pawn->SetCustomData(i, FourDataPieces, false);
		x += 400;
	}
	ISM_Pawn->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_Pawn->SetCustomData(31, FourDataPieces, true);
	/* ROOK */
	x = 1400.f; y = 5400.f;
	for (int i = 6; i < 8; i++) {
		ISM_Rook->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
		ISM_Rook->SetCustomData(i, FourDataPieces, true);
		x += 2800.f;
	}
	/* KNIGHT */
	x = 1800.f; y = 5400.f;
	for (int i = 6; i < 8; i++) {
		ISM_Knight->AddInstanceWorldSpace(FTransform(FRotator(0.f,-90.f,0.f),FVector(x, y, z),FVector(1.f)));
		ISM_Knight->SetCustomData(i, TeamColors, true);
		x += 2000.f;
	}
	/* BISHOP */
	x = 2200.f; y = 5400.f;
	for (int i = 6; i < 8; i++) {
		ISM_Bishop->AddInstanceWorldSpace(FTransform(FRotator(0.f,-90.f,0.f),FVector(x, y, z),FVector(1.f)));
		ISM_Bishop->SetCustomData(i, TeamColors, true);
		x += 1200.f;
	}
	/* QUEEN */
	x = 3000.f; y = 5400.f;
	ISM_Queen->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_Queen->SetCustomData(3, TeamColors, true);
	/* KING */
	x = 2600.f; y = 5400.f;
	ISM_King->AddInstanceWorldSpace(FTransform(FVector(x, y, z)));
	ISM_King->SetCustomData(3, FourDataPieces, true);

}

// Called when the game starts or when spawned
void AChessPieces::BeginPlay()
{
	Super::BeginPlay();

	/* MOVING INSTANCE MECHANISM
	ISM_Pawn->GetInstanceTransform(0, OUT CurrentTransform, true);
	NewLocation = CurrentTransform.GetLocation() + FVector(400.f, 0.f, 0.f);
	ISM_Pawn->UpdateInstanceTransform(0, FTransform(NewLocation), true, true);
	 */
}
