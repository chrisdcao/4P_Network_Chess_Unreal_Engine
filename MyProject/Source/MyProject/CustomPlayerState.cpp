// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerState.h"

#include "Engine/DemoNetDriver.h"

ACustomPlayerState::ACustomPlayerState()
{

}

ACustomPlayerState::ACustomPlayerState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}

void ACustomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACustomPlayerState, playerIndex);
}
