// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentActor.generated.h"

UCLASS()
class MYPROJECT_API AParentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


/* FUNCTIONS */
public:

	UFUNCTION()
	void SetMaterialInactive(const int8& PlayerIndex);

	UFUNCTION()
	void SetMaterialActive(const int8& PlayerIndex);


/* PROPERTIES */

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UMaterial* BottomActiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* BottomInactiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* TopActiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* TopInactiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* LeftActiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* LeftInactiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* RightActiveMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* RightInactiveMaterial;

};
