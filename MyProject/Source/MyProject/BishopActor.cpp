// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopActor.h"

// Sets default values
ABishopActor::ABishopActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BishopMeshAsset(TEXT("StaticMesh'/Game/DuChamp/Bishop_DuChamp.Bishop_DuChamp'"));

	if (BishopMeshAsset.Succeeded())
		Mesh->SetStaticMesh(BishopMeshAsset.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Mesh Found for Bishop!"));
}

// Called when the game starts or when spawned
void ABishopActor::BeginPlay()
{
	Super::BeginPlay();
	
}
