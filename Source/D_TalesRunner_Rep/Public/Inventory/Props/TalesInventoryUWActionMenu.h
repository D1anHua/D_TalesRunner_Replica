// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesInventoryUWActionMenu.generated.h"

class UTalesSlotUserWidget;
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

	//! @设置触发者, 需要在创建后调用
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetInstigatorActor(UTalesSlotUserWidget* InstigatorActor){ this->InstigatorSlot = InstigatorActor; };

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
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void EquipClick();
	UFUNCTION()
	void EquipHover();
	UFUNCTION()
	void EquipUnHover();
	
	UFUNCTION()
	void DropClick();
	UFUNCTION()
	void DropHover();
	UFUNCTION()
	void DropUnHover();
	
	UFUNCTION()
	void CancelHover();
	UFUNCTION()
	void CancelUnHover();

	UPROPERTY(Transient)
	UTalesSlotUserWidget* InstigatorSlot;	
};
