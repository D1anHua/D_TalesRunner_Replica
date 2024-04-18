// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesHealthBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesHealthBarUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* HealthBarWarpBox;
	UPROPERTY(BlueprintReadWrite ,EditAnywhere, Category = "Child")
	TSubclassOf<class UTalesHeartUserWidget> HeartWidget;

	float MaxHealth = 0.f;
	float NowHealth = MaxHealth;
};
