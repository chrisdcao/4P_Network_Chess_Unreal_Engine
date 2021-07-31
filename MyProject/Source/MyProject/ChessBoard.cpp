// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessBoard.h"

AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// ChessBoard Setup
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent"));
	SetRootComponent(InstancedStaticMeshComponent);
	InstancedStaticMeshComponent->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComponent->NumCustomDataFloats = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Surroundings/Floor.Floor'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Materials/M_TestMaterial.M_TestMaterial'"));

	InstancedStaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	InstancedStaticMeshComponent->SetMaterial(0, Material.Object);

	for (int i = 0; i < 196; i++) 
	{
		// we init col first then row: i%14 = col, i/14 = row
		InstancedStaticMeshComponent->AddInstanceWorldSpace(FTransform(FVector(i/14 * 400, i%14 * 400, 0.f)));
		// TODO: mark what Value is What color (create a color chart like Chess Piece Lerp)
		if (XOR(i % 2, i / 14 % 2))
			InstancedStaticMeshComponent->SetCustomDataValue(i, 0, 1.f, true);
		else
			InstancedStaticMeshComponent->SetCustomDataValue(i, 1, 1.f, true);
	}

	for (int i = 0; i >= 0 && i <= 2; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 11; i >= 11 && i <= 16; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 25; i >= 25 && i <= 30; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 39; i >= 39 && i <= 41; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 154; i >= 154 && i <= 156; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 165; i >= 165 && i <= 170; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 179; i >= 179 && i <= 184; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);
	for (int i = 193; i >= 193 && i <= 195; i++)
		InstancedStaticMeshComponent->UpdateInstanceTransform(i, FTransform().GetScaled(0.f), true);

}

void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
}
