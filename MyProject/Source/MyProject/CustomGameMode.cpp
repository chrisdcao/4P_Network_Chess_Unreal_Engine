// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"

#include "CustomPawn.h"

ACustomGameMode::ACustomGameMode() 
{

	count = 1;
}

void ACustomGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void ACustomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ACustomPawn* NewPawn = Cast<ACustomPawn>(NewPlayer->GetPawn());
	if (NewPawn)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Yellow, TEXT("Pawn exists"));
		}
	}
}
