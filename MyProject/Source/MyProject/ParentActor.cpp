// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentActor.h"

// Sets default values
AParentActor::AParentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>BottomInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_Red.M_Red'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>TopInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/M_Yellow.M_Yellow'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>LeftInactiveMaterialAsset(TEXT("Material'/Game/Materials/M_Blue.M_Blue'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>RightInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Materials/Green.Green'"));

	static ConstructorHelpers::FObjectFinder<UMaterial>BottomActiveMaterialAsset(TEXT("Material'/Game/Materials/M_ChessPieces.M_ChessPieces'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>TopActiveMaterialAsset(TEXT("Material'/Game/Materials/M_ChessPieces.M_ChessPieces'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>LeftActiveMaterialAsset(TEXT("Material'/Game/Materials/M_ChessPieces.M_ChessPieces'"));
	static ConstructorHelpers::FObjectFinder<UMaterial>RightActiveMaterialAsset(TEXT("Material'/Game/Materials/M_ChessPieces.M_ChessPieces'"));

	BottomInactiveMaterial = BottomInactiveMaterialAsset.Object;
	TopInactiveMaterial = TopInactiveMaterialAsset.Object;
	LeftInactiveMaterial = LeftInactiveMaterialAsset.Object;
	RightInactiveMaterial = RightInactiveMaterialAsset.Object;
	
	BottomActiveMaterial = BottomActiveMaterialAsset.Object;
	TopActiveMaterial = TopActiveMaterialAsset.Object;
	LeftActiveMaterial = LeftActiveMaterialAsset.Object;
	RightActiveMaterial = RightActiveMaterialAsset.Object;

}

// Called when the game starts or when spawned
void AParentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AParentActor::SetMaterialInactive(const int8& playerIndex)
{
	if (playerIndex == 1)
		Mesh->SetMaterial(0, BottomInactiveMaterial);
	else if (playerIndex == 2)
		Mesh->SetMaterial(0, LeftInactiveMaterial);
	else if (playerIndex == 3)
		Mesh->SetMaterial(0, TopInactiveMaterial);
	else if (playerIndex == 4)
		Mesh->SetMaterial(0, RightInactiveMaterial);
}

void AParentActor::SetMaterialActive(const int8& playerIndex)
{
	if (playerIndex == 1)
		Mesh->SetMaterial(0, BottomActiveMaterial);
	else if (playerIndex == 2)
		Mesh->SetMaterial(0, LeftActiveMaterial);
	else if (playerIndex == 3)
		Mesh->SetMaterial(0, TopActiveMaterial);
	else if (playerIndex == 4)
		Mesh->SetMaterial(0, RightActiveMaterial);
}
