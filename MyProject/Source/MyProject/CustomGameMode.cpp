// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"

#include "CustomGameState.h"
#include "CustomPlayerController2.h"
#include "PlayerPawn.h"

// WHEN GOING ONLINE, GAME MODE ONLY EXISTS IN SERVER
ACustomGameMode::ACustomGameMode() 
{
    DefaultPawnClass = APlayerPawn::StaticClass();
    PlayerStateClass = ACustomGameState::StaticClass();
    PlayerControllerClass = ACustomPlayerController2::StaticClass();
    count = 1;
}

void ACustomGameMode::PostLogin(APlayerController* NewPlayer) {
    Super::PostLogin(NewPlayer);
    // Set net player index only works with same-base connection (same internet/splitscreen player)
    // when this becomes official online game we will set indices through CreateSession or player ID
    if (count == 1) 
    {
        NewPlayer->NetPlayerIndex = 1;
        count++;
    }
    else if (count == 2) 
    {
        NewPlayer->NetPlayerIndex = 2;
        count++;
    }
    else if (count == 3) 
    {
        NewPlayer->NetPlayerIndex = 3;
        count++;
    }
    else if (count == 4) 
    {
        NewPlayer->NetPlayerIndex = 4;
        count++;
    }
    UE_LOG(LogTemp, Warning, TEXT("Current Net Index: %d"), NewPlayer->NetPlayerIndex);
}
