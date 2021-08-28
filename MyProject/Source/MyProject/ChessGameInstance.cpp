// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameInstance.h"

UChessGameInstance::UChessGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void UChessGameInstance::Init()
{
	Super::Init();
}

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

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

