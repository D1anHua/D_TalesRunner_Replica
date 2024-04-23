// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesInventoryUWActionMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UTalesInventoryUWActionMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(CancelButton)
	{
		EquipButton->OnHovered.AddDynamic(this, &ThisClass::EquipHover);
		EquipButton->OnUnhovered.AddDynamic(this, &ThisClass::EquipUnHover);
		
		DropButton->OnHovered.AddDynamic(this, &ThisClass::DropHover);
		DropButton->OnUnhovered.AddDynamic(this, &ThisClass::DropUnHover);
		
		CancelButton->OnClicked.AddDynamic(this, &ThisClass::RemoveFromParent);
		CancelButton->OnHovered.AddDynamic(this, &ThisClass::CancelHover);
		CancelButton->OnUnhovered.AddDynamic(this, &ThisClass::CancelUnHover);
	}
}

void UTalesInventoryUWActionMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if(ActionMenuSizeBox)
	{
		// auto CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ActionMenuSizeBox);
		auto MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		ActionMenuSizeBox->SetRenderTranslation(MousePosition);	
	}
}

void UTalesInventoryUWActionMenu::SetText(FText Text)
{
	if(EquipText)
	{
		EquipText->SetText(Text);
	}
}

void UTalesInventoryUWActionMenu::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

void UTalesInventoryUWActionMenu::EquipHover()
{
	if(EquipHoverImage)
	{
		EquipHoverImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTalesInventoryUWActionMenu::EquipUnHover()
{
	if(EquipHoverImage)
	{
		EquipHoverImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTalesInventoryUWActionMenu::DropHover()
{
	if(DropHoverImage)
	{
		DropHoverImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTalesInventoryUWActionMenu::DropUnHover()
{
	if(DropHoverImage)
	{
		DropHoverImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTalesInventoryUWActionMenu::CancelHover()
{
	if(CancelHoverImage)
	{
		CancelHoverImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTalesInventoryUWActionMenu::CancelUnHover()
{
	if(CancelHoverImage)
	{
		CancelHoverImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

