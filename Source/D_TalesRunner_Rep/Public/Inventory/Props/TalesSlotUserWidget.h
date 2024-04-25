// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/TalesInventoroyItem.h"
#include "Inventory/TalesInventoryComponent.h"
#include "Inventory/TalesInventoryUserWidget.h"
#include "TalesSlotUserWidget.generated.h"

class UTalesInventoryUWActionMenu;
class UTalesInventoryComponent;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesSlotUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
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
	
	UPROPERTY(meta = (BindWidget))
	class UImage* EquipBackgroundImage;

	//! @brief QuantitySizeBox: 右下角小盒子, 用来显示数字
	UPROPERTY(meta = (BindWidget))
	class USizeBox* QuantitySizeBox;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNumber;

protected:
#pragma region Data
	//! Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	struct FTalesInventoryItemSlot Data;

	UPROPERTY(Transient)
	UTalesInventoryComponent* InventoryComponent;
	UPROPERTY(Transient)
	class ATalesCharacter* OwningCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Data")
	TSubclassOf<UTalesInventoryUWActionMenu> ActionMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ATalesInventoryItem> ItemToBind;
#pragma endregion

#pragma region Widget
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* UnHoverBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* HoverBorder;
	
#pragma endregion

	
	UFUNCTION()
	void OnClick();
	UFUNCTION()
	void OnHovered();
	UFUNCTION()
	void OnUnHovered();
public:
	// HelpFunction
	void SetDataConstruct(FTalesInventoryItemSlot OtherData);
	void ActivateSlot();
	void UnActivateSlot();
	void GetInventoryComponent();
	void UseThisItem();
	void DropThisItem();
	inline FTalesInventoryItemSlot GetData() const{return Data;}

private:
	void UseFood();
	bool IsActivate();
	int GetSourcePower();
};

