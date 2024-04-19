// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Inventory/TalesInventoryMainLeftUserWidget.h"
#include "TalesItemButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickDelegate, FString, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHoverDelegate, FString, Name);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnHoverDelegate, FString, Name);
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesItemButton : public UButton
{
	GENERATED_BODY()

public:
	UTalesItemButton();
	UPROPERTY()
	FClickDelegate ClickDelegate;
	UPROPERTY()
	FHoverDelegate HoverDelegate;
	UPROPERTY()
	FUnHoverDelegate UnHoverDelegate;

	UFUNCTION()
	void OnClick();
	UFUNCTION()
	void OnHover();
	UFUNCTION()
	void OnUnHover();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name", meta = (GetOptions="GetButtonName"))
	FString Name;
	
	// Helper Functions
	UFUNCTION()
	FORCEINLINE TArray<FString> GetButtonName() const{ return BUTTON_ITEM_NAME; };
};
