// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnLevelUpWidget.h"

UPawnLevelUpWidget::UPawnLevelUpWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPawnLevelUpWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPawnLevelUpWidget::updateComboCount(int32 Value)
{
	if (TXTCombo && Value > 1)	/// every time the combo > 1 -> display
	{ // turn textbox visible if already hidden
		if (TXTCombo->Visibility == ESlateVisibility::Hidden)
			TXTCombo->SetVisibility(ESlateVisibility::Visible);
	}
	// generate output like 2x Combo
	TXTCombo->SetText(FText::FromString(FString::FromInt(Value) + "x Combo"));
}

void UPawnLevelUpWidget::ResetCombo()
{
	if (TXTCombo)
	{
		TXTCombo->SetVisibility(ESlateVisibility::Hidden);
	}

}
