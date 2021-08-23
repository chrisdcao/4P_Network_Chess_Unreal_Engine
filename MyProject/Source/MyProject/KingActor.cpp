// Fill out your copyright notice in the Description page of Project Settings.


#include "KingActor.h"

// Sets default values
AKingActor::AKingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	firstMove = 1;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>KingMeshAsset(TEXT("StaticMesh'/Game/DuChamp/King_DuChamp.King_DuChamp'"));

	if (KingMeshAsset.Succeeded())
		Mesh->SetStaticMesh(KingMeshAsset.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Mesh Found for King!"));

}

// Called when the game starts or when sKinged
void AKingActor::BeginPlay()
{
	Super::BeginPlay();
	
}
