// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TalesInventoroyItem.h"
#include "Blueprint/UserWidget.h"
#include "Props/TalesSlotUserWidget.h"
#include "TalesInventoryMainLeftUserWidget.generated.h"

#define BUTTON_ITEM_NAME {"Sward", "Shield", "Eatable"}

class UTalesSlotUserWidget;
struct FTalesInventoryItemSlot;
class ATalesCharacter;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesInventoryMainLeftUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	inline UTalesSlotUserWidget* GetSwardSlotOnActivate() const {return EquipSwardSlot;}
	inline UTalesSlotUserWidget* GetShieldSlotOnActivate() const {return EquipShieldSlot;}
	void SetSwardSlotOnActivate(UTalesSlotUserWidget* NewSwardSlot);
	void SetShieldSlotOnActivate(UTalesSlotUserWidget* NewShieldSlot);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PageSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* SwardWrapBox;
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* ShieldWrapBox;
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* EatableWrapBox;
	UPROPERTY(BlueprintReadWrite ,EditAnywhere, Category = "Slot")
	TSubclassOf<class UTalesSlotUserWidget> SlotClass;
	
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
	
	TArray<UTalesItemButton*> ButtonItems;
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

	UPROPERTY()
	ATalesCharacter* TalesCharacterOwner;

	void InitializeData();
	void InitializeOnePageData(TMultiMap<FName, FTalesInventoryItemSlot>& PageData, UWrapBox*& Box, FTalesInventoryItemSlot ActivateSlot, UTalesSlotUserWidget*& EquipSlot);
	void InitializeOnePageData_Eatable(TMultiMap<FName, FTalesInventoryItemSlot>& PageData, UWrapBox*& Box);
	void SetTalesCharacterOwner();

	UPROPERTY(Transient)
	UTalesSlotUserWidget* EquipSwardSlot;
	UPROPERTY(Transient)
	UTalesSlotUserWidget* EquipShieldSlot;
};