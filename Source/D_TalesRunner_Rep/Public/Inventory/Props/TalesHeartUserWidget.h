// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesHeartUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesHeartUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;
	void SetImage(float HeartSize);

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* HeartImage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HeartNum")
	float HeartNum = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Texture")
	UTexture2D* HeartFull;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Texture")
	UTexture2D* HeartHalfFull;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Texture")
	UTexture2D* HeartEmpty;
};
