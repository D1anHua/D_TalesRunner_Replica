// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalesInventoryMainLeftUserWidget.generated.h"

#define BUTTON_ITEM_NAME {"Sward", "Shield", "Eatable"}

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesInventoryMainLeftUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
protected:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PageSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	class UTalesItemButton* SwardButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* SwardImage;

	UPROPERTY(meta = (BindWidget))
	class UTalesItemButton* ShieldButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* ShieldImage;

	UPROPERTY(meta = (BindWidget))
	class UTalesItemButton* EatableButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* EatableImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
	struct FLinearColor HoveredColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
	struct FLinearColor ActivatedColor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Color")
	struct FLinearColor NotActivatedColor;
	
	UPROPERTY()
	TArray<UTalesItemButton*> ButtonItems;
	UPROPERTY()
	TArray<UImage*> ImageItems;

	UFUNCTION()
	void OnItemButtonHovered(FString Name);
	UFUNCTION()
	void OnItemButtonUnHovered(FString Name);
	UFUNCTION()
	void OnItemButtonClicked(FString Name);

	UFUNCTION(BlueprintCallable)
	void ActivateButton(int Index);
private:
	TArray<FString> ButtonItemNames = BUTTON_ITEM_NAME;
	FORCEINLINE int GetIndex(FString Name) const{ return ButtonItemNames.Find(Name); };
};