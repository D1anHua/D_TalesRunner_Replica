// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/TalesInventoroyItem.h"
#include "TalesSlotUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesSlotUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	
	//! @brief Button: will rewrite the hover, un hover, selected function
	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;
	//! @brief Border: Use when this Button is hover or something will change
	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotSelectBorder;
	//! @brief  HoverBorderImage: When Hover it will visiable
	UPROPERTY(meta = (BindWidget))
	class UImage* HoverBorderImage;

	//! @brief ItemImage: the main Texture for this Slot
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	//! @brief QuantitySizeBox: 右下角小盒子, 用来显示数字
	UPROPERTY(meta = (BindWidget))
	class USizeBox* QuantitySizeBox;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNumber;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	struct FTalesInventoryItemSlot Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* UnHoverBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* HoverBorder;
	
	UFUNCTION()
	void OnHovered();
	UFUNCTION()
	void OnUnHovered();

public:
	// HelpFunction
	void SetDataConstruct(FTalesInventoryItemSlot OtherData);
};
