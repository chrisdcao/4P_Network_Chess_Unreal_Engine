// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "CustomPlayerState.h"
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
    if (count == 1) 
    {
        NewPlayer->GetPawn()->GetPlayerState()->SetPlayerId(1);
        count++;
    }
    else if (count == 2) 
    {
        NewPlayer->GetPawn()->GetPlayerState()->SetPlayerId(-2);
        count++;
    }
    else if (count == 3) 
    {
        NewPlayer->GetPawn()->GetPlayerState()->SetPlayerId(3);
        count++;
    }
    else if (count == 4) 
    {
        NewPlayer->GetPawn()->GetPlayerState()->SetPlayerId(-4);
        count++;
    }
}
