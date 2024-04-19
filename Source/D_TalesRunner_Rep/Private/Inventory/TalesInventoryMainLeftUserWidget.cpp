// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/TalesInventoryMainLeftUserWidget.h"

#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Inventory/Props/TalesItemButton.h"


void UTalesInventoryMainLeftUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	ButtonItems = {SwardButton, ShieldButton, EatableButton};
	ImageItems = {SwardImage, ShieldImage, EatableImage};
	for(auto& item : ButtonItems)
	{
		if(item)
		{
			item->HoverDelegate.AddDynamic(this, &UTalesInventoryMainLeftUserWidget::OnItemButtonHovered);
			item->UnHoverDelegate.AddDynamic(this, &ThisClass::OnItemButtonUnHovered);
			item->ClickDelegate.AddDynamic(this, &ThisClass::OnItemButtonClicked);
		}
	}
	int Index = PageSwitcher->GetActiveWidgetIndex();
	if(Index != INDEX_NONE)
	{
		ActivateButton(Index);
	}
}
void UTalesInventoryMainLeftUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTalesInventoryMainLeftUserWidget::OnItemButtonHovered(FString Name)
{
	int Index = GetIndex(Name);
	if(Index != INDEX_NONE)
	{
		ImageItems[Index]->SetColorAndOpacity(HoveredColor);		
	}
}

void UTalesInventoryMainLeftUserWidget::OnItemButtonUnHovered(FString Name)
{
	int Index = GetIndex(Name);
	if(Index != INDEX_NONE)
	{
		ImageItems[Index]->SetColorAndOpacity(ActivatedColor);		
	}
}

void UTalesInventoryMainLeftUserWidget::OnItemButtonClicked(FString Name)
{
	int Index = GetIndex(Name);
	if(Index != INDEX_NONE)
	{
		ActivateButton(Index);
	}
}

void UTalesInventoryMainLeftUserWidget::ActivateButton(int Index)
{
	for(int i = 0; i < ButtonItemNames.Num(); ++i)
	{
		if(i == Index)
		{
			ImageItems[i]->SetBrushTintColor(ActivatedColor);
			PageSwitcher->SetActiveWidgetIndex(Index);
		}else
		{
			ImageItems[i]->SetBrushTintColor(NotActivatedColor);
		}
	}
}
