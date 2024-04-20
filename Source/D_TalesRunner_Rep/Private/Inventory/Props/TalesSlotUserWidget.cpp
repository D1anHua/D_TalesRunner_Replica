// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesSlotUserWidget.h"

#include "Character/TalesCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Inventory/TalesInventoryComponent.h"
#include "Inventory/TalesInventoryUserWidget.h"
#include "Inventory/Props/TalesInfoCueUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UTalesSlotUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(SlotButton)
	{
		SlotButton->OnHovered.AddDynamic(this, &ThisClass::OnHovered);
		SlotButton->OnUnhovered.AddDynamic(this, &ThisClass::OnUnHovered);
	}
	GetInventoryComponent();
}

void UTalesSlotUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetDataConstruct(Data);
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
	if(Data.Quantity <= 0)
	{
		return;
	}
	auto InfoCueWidget = InventoryComponent->GetInventoryUserWidget()->GetInfoCueUserWidget();
	InfoCueWidget->InitializeAllInfo(0, Data);
	InfoCueWidget->SetVisibility(ESlateVisibility::Visible);
}

void UTalesSlotUserWidget::OnUnHovered()
{
	HoverBorderImage->SetVisibility(ESlateVisibility::Hidden);	
	SlotSelectBorder->SetBrushFromTexture(UnHoverBorder);
	SlotSelectBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.4f));
	auto InfoCueWidget = InventoryComponent->GetInventoryUserWidget()->GetInfoCueUserWidget();
	InfoCueWidget->SetVisibility(ESlateVisibility::Hidden);
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

void UTalesSlotUserWidget::GetInventoryComponent()
{
	auto Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ATalesCharacter* TalesCharacter = Cast<ATalesCharacter>(Character);
	if(TalesCharacter)
	{
		InventoryComponent = TalesCharacter->GetTalesInventoryComponent();
		if(!IsValid(InventoryComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("Error in Slot User Widget: Unable to Get Inventory Componet"));
		}
	}	
}
