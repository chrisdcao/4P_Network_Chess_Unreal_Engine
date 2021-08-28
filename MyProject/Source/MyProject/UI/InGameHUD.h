// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnLevelUpWidget.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AInGameHUD : public AHUD
{
	GENERATED_BODY()

public:	
	AInGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpdateComboCount(int32 Value);

	UFUNCTION()
	void ResetCombo();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ComboWidgetClass;

private:
	UPawnLevelUpWidget* ComboWidget;
};
