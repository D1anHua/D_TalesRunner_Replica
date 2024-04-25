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
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	inline void SetHealth(float InMaxHealth, float InNowHealth)
	{
		this->MaxHealth = InMaxHealth;
		this->NowHealth = InNowHealth;
	};

	void AddHelathRuntime(float NowHealth);

protected:
	UPROPERTY(meta=(BindWidget))
	class UWrapBox* HealthBarWarpBox;
	UPROPERTY(BlueprintReadWrite ,EditAnywhere, Category = "Child")
	TSubclassOf<class UTalesHeartUserWidget> HeartWidget;

	float MaxHealth = 0.f;
	float NowHealth = MaxHealth;

private:
	void InitialHealth(float InMaxHealth, float InNowHealth);
};
