// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PawnLevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UPawnLevelUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPawnLevelUpWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void updateComboCount(int32 Value);

	void ResetCombo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TXTCombo;

};
