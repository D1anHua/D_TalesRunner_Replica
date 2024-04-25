// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesInfoCueUserWidget.h"
#include "Inventory/TalesInventoroyItem.h"
#include "Inventory/Props/TalesHealthBarUserWidget.h"
#include "Inventory/Props/TalesInventoryPropsPowerSward.h"
#include "Components/Image.h"
#include "Components/NamedSlot.h"
#include "Components/TextBlock.h"


void UTalesInfoCueUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	// InitializeAllInfo(0, DataPoint);
}

void UTalesInfoCueUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ActivateImage->SetVisibility(ESlateVisibility::Hidden);
	InitializeAllInfo(0, DataPoint);
}

void UTalesInfoCueUserWidget::InitializeAllInfo(int SourcePowerData, FTalesInventoryItemSlot Item)
{
	if(ImageInfo)
	{
		PowerNamedSlot->ClearChildren();
		// Set Text
		float Power = 0.f;
		auto Row = Item.ItemRowHandle.GetRow<FTalesInventoryMenuItem>("Searching for TEXT");
		if(Row)
		{
			NameInfo->SetText(Row->Name);
			DetailInfo->SetText(Row->Description);
			Power = Row->Power;
		}
		
		switch (Item.ItemType)
		{
		case Sward:
			if(ensureAlways(SwardTexture))
			{
				ImageInfo->SetBrushFromTexture(SwardTexture);
				if(ensureAlways(IntPowerClass))
				{
					auto Widget = CreateWidget<UTalesInventoryPropsPowerSward>(GetWorld(), IntPowerClass);
					Widget->SetPower(SourcePowerData, FMath::Floor(Power));
					PowerNamedSlot->AddChild(Widget);
				}
			}
			break;
		case Shield:
			if(ensureAlways(ShieldTexture))
			{
				ImageInfo->SetBrushFromTexture(ShieldTexture);
				if(ensureAlways(IntPowerClass))
				{
					auto Widget = CreateWidget<UTalesInventoryPropsPowerSward>(GetWorld(), IntPowerClass);
					Widget->SetPower(SourcePowerData, FMath::Floor(Power));
					PowerNamedSlot->AddChild(Widget);
				}
			}
			break;
		case Eatable:
			if(ensureAlways(EatableTexture))
			{
				ImageInfo->SetBrushFromTexture(EatableTexture);
				if(ensureAlways(EatablePowerClass))
				{
					auto Widget = CreateWidget<UTalesHealthBarUserWidget>(GetWorld(), EatablePowerClass);
					Widget->SetHealth(FMath::CeilToInt(Power), Power);
					Widget->SetRenderTransform({FVector2d(-70.,0.), FVector2d(0.7,0.7), FVector2d(0.0,0.), 0.});
					PowerNamedSlot->AddChild(Widget);
				}
			}
			break;
		default:
			break;
		}
	}

}
