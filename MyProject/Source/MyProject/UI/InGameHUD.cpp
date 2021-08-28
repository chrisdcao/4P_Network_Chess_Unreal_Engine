// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

AInGameHUD::AInGameHUD()
{

}

void AInGameHUD::UpdateComboCount(int32 Value)
{
	if (ComboWidget)
	{
		ComboWidget->updateComboCount(Value);
	}
}

void AInGameHUD::ResetCombo()
{
	if (ComboWidget)
	{
		ComboWidget->ResetCombo();
	}
}

void AInGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AInGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AInGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ComboWidget)
	{
		ComboWidget = CreateWidget<UPawnLevelUpWidget>(GetWorld(), ComboWidgetClass);
		if (ComboWidget)
		{
			ComboWidget->AddToViewport();
		}
	}
}
