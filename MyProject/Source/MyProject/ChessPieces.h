// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "ChessPieces.generated.h"

UCLASS()
class MYPROJECT_API AChessPieces : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPieces();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	FTransform CurrentTransform;

	FVector NewLocation;

	FVector MoveValue;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* ISM_Pawn;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* ISM_Rook;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* ISM_Knight;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* ISM_Bishop;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* ISM_King;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* ISM_Queen;

	UPROPERTY(Transient)
	TArray<int32> ChessBoardValues;
};
