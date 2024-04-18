// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesInventoryUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesInventoryUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MoneyAmountText;

	UFUNCTION()
	void MoneyAmountUpdate(AActor* InstigateActor, UTalesInventoryComponent* OwnComp, int32 MoneyAmount, int32 delta);
};
