// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameInstance.h"
#include "SpawnManager.h"
#include "Kismet/GameplayStatics.h"

UChessGameInstance::UChessGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void UChessGameInstance::Init()
{
	Super::Init();
	// since each player will play in their own game window of chess, num of local players at each window (each game instance) will be 1
//	GetFirstGamePlayer()->GetGameInstance()->

}

// EACH GAME WINDOW CREATED HOLDS ITSELF AN INSTANCE OF THE GAME (a.k.a A GAME INSTANCE)
// -> 4 player spawned in 4 separate windows will have each instance of their own, while 4 person respawning in 1 game window will only have 1 game instance
// so should I store my array in the Game Instance instead of the pawn? Pawn is the character in game and thus might not make much sense to hold static things of game?
// Local players are players that play within the same Game Window (under 1 Game Instance), while players playing in different windows aren't considered local players (like this chess game will have local = 1, or dota 2 too where each hero 'player' runs their own game window to play instead of sharing under one window)
void UChessGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!World) return;

	World->ServerTravel("/Game/NewMap_NoFloor?listen");
}

void UChessGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	// which is also the only player at that machine because we only allow player to play game in 1 window (not sharing)
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

