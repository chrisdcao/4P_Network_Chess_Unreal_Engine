// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentActor.h"

// Sets default values
AParentActor::AParentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
    // bReplicates = true;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>BottomInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Megascans/Surfaces/Brick_Bond_Cedar_Parquet_ticmabav/Brick_Bond_Cedar_Parquet_ticmabav_inst.Brick_Bond_Cedar_Parquet_ticmabav_inst'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>TopInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Megascans/Surfaces/Varnished_Madrone_Burl_Veneer_tcnqaeoc/Varnished_Madrone_Burl_Veneer_tcnqaeoc_inst.Varnished_Madrone_Burl_Veneer_tcnqaeoc_inst'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>LeftInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Megascans/Surfaces/Brick_Bond_Walnut_Parquet_ucmicfjl/Brick_Bond_Walnut_Parquet_ucmicfjl_inst.Brick_Bond_Walnut_Parquet_ucmicfjl_inst'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>RightInactiveMaterialAsset(TEXT("MaterialInstanceConstant'/Game/Megascans/Surfaces/Varnished_Elm_Burl_Veneer_tcicdebc/Varnished_Elm_Burl_Veneer_tcicdebc_inst.Varnished_Elm_Burl_Veneer_tcicdebc_inst'"));

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

    //if (HasAuthority())
        //NetUpdateFrequency = 1;
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
