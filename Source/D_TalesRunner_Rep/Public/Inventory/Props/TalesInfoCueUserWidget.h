// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Inventory/TalesInventoroyItem.h"
#include "TalesInfoCueUserWidget.generated.h"

class UTalesInventoryPropsPowerSward;
class UTalesHealthBarUserWidget;
class UImage;
class UTextBlock;
/**
 * @brief 用来通知详细信息的User Widget, 在界面右下角
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesInfoCueUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameInfo;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DetailInfo;
	UPROPERTY(meta = (BindWidget))
	UNamedSlot* PowerNamedSlot;
	UPROPERTY(meta = (BindWidget))
	UImage* ImageInfo;
	UPROPERTY(meta = (BindWidget))
	UImage* ActivateImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Texture")
	UTexture2D* SwardTexture;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Texture")
	UTexture2D* ShieldTexture;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Texture")
	UTexture2D* EatableTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NamedSlot")
	TSubclassOf<UTalesInventoryPropsPowerSward> IntPowerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NamedSlot")
	TSubclassOf<UTalesHealthBarUserWidget> EatablePowerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Texture")
	FTalesInventoryItemSlot DataPoint;	
	
public:
	void InitializeAllInfo(int SourcePowerData, FTalesInventoryItemSlot Item);

public:
	inline void SetActivate(){ ActivateImage->SetVisibility(ESlateVisibility::Visible);}
	inline void SetUnActivate(){ ActivateImage->SetVisibility(ESlateVisibility::Hidden);}
};
