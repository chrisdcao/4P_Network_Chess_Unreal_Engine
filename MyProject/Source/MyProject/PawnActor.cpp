// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnActor.h"

// Sets default values
APawnActor::APawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PawnMeshAsset(TEXT("StaticMesh'/Game/DuChamp/Pawn_DuChamp.Pawn_DuChamp'"));

	if (PawnMeshAsset.Succeeded())
		Mesh->SetStaticMesh(PawnMeshAsset.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Mesh Found for Pawn!"));

}

// Called when the game starts or when spawned
void APawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}
