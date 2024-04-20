// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesSlotUserWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UTalesSlotUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetDataConstruct(Data);
	SlotButton->OnHovered.AddDynamic(this, &ThisClass::OnHovered);
	SlotButton->OnUnhovered.AddDynamic(this, &ThisClass::OnUnHovered);
	if(IsValid(HoverBorder) && IsValid(UnHoverBorder))
	{
		HoverBorderImage->SetVisibility(ESlateVisibility::Hidden);	
		SlotSelectBorder->SetBrushFromTexture(UnHoverBorder);
		SlotSelectBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.4f));
	}
}

void UTalesSlotUserWidget::OnHovered()
{
	HoverBorderImage->SetVisibility(ESlateVisibility::Visible);	
	SlotSelectBorder->SetBrushFromTexture(HoverBorder);
	SlotSelectBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
}

void UTalesSlotUserWidget::OnUnHovered()
{
	HoverBorderImage->SetVisibility(ESlateVisibility::Hidden);	
	SlotSelectBorder->SetBrushFromTexture(UnHoverBorder);
	SlotSelectBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.4f));
}

void UTalesSlotUserWidget::SetDataConstruct(FTalesInventoryItemSlot OtherData)
{
	this->Data = OtherData;
	if(Data.Quantity > 0)
	{
		if(!Data.ItemRowHandle.IsNull())
		{
			ItemNumber->SetText(FText::FromString(FString::FromInt(Data.Quantity)));
			QuantitySizeBox->SetVisibility(ESlateVisibility::Visible);
			FTalesInventoryMenuItem* Row = Data.ItemRowHandle.GetRow<FTalesInventoryMenuItem>(FString("Searching for Textrue"));
			if(Row)
			{
				ItemImage->SetBrushFromTexture(Row->Thumbnails);
			}
			ItemImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		// 取消quantity Size Box的显示
		QuantitySizeBox->SetVisibility(ESlateVisibility::Hidden);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

