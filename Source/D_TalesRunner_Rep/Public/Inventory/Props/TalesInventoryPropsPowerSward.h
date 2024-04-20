// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesInventoryPropsPowerSward.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesInventoryPropsPowerSward : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void SetPower(int Source, int Target);
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SourcePower;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TargetPower;
};
