// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ParentActor.h"
#include "GameFramework/Actor.h"
#include "QueenActor.generated.h"

UCLASS()
class MYPROJECT_API AQueenActor : public AParentActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQueenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
