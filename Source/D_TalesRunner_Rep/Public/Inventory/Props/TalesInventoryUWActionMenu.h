// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesInventoryUWActionMenu.generated.h"

class USizeBox;
class UButton;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesInventoryUWActionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void SetText(FText Text);

	UPROPERTY(meta= (BindWidget))
	USizeBox* ActionMenuSizeBox;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EquipText;

	UPROPERTY(meta = (BindWidget))
	UButton* EquipButton;
	UPROPERTY(meta = (BindWidget))
	UImage* EquipHoverImage;
	UPROPERTY(meta = (BindWidget))
	UButton* DropButton;
	UPROPERTY(meta = (BindWidget))
	UImage* DropHoverImage;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UImage* CancelHoverImage;

protected:
	void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void EquipHover();
	UFUNCTION()
	void EquipUnHover();
	
	UFUNCTION()
	void DropHover();
	UFUNCTION()
	void DropUnHover();
	
	UFUNCTION()
	void CancelHover();
	UFUNCTION()
	void CancelUnHover();
};
