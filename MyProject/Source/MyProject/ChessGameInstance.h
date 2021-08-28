// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChessGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UChessGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UChessGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	// make the console command call to Host() function (using Exec in FUNCTION)
	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& Address);
	
};
