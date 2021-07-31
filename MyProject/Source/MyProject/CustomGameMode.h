// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACustomGameMode();

protected:

	virtual void BeginPlay() override;
	
};
