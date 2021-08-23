// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightActor.h"

// Sets default values
AKnightActor::AKnightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>KnightMeshAsset(TEXT("StaticMesh'/Game/DuChamp/Knight_DuChamp.Knight_DuChamp'"));

	if (KnightMeshAsset.Succeeded())
		Mesh->SetStaticMesh(KnightMeshAsset.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("No Mesh Found for Knight!"));
}

// Called when the game starts or when spawned
void AKnightActor::BeginPlay()
{
	Super::BeginPlay();

}
