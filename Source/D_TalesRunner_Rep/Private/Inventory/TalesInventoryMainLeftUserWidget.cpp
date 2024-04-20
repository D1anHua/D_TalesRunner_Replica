// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/TalesInventoryMainLeftUserWidget.h"

#include "Character/TalesCharacter.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "Inventory/Props/TalesSlotUserWidget.h"
#include "Inventory/TalesInventoryComponent.h"
#include "Inventory/Props/TalesItemButton.h"
#include "Kismet/GameplayStatics.h"


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
	SetTalesCharacterOwner();
}
void UTalesInventoryMainLeftUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeData();
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

void UTalesInventoryMainLeftUserWidget::InitializeData()
{
	if(TalesCharacterOwner && ensureAlways(SlotClass))
	{
		FTalesInventoryPackageDatas Datas = TalesCharacterOwner->GetTalesInventoryComponent()->GetPackagesDatas();	
		// Sward
		InitializeOnePageData(Datas.Sward, SwardWrapBox);
		InitializeOnePageData(Datas.Shield, ShieldWrapBox);
		InitializeOnePageData(Datas.Eatable, EatableWrapBox);
	}
}

void UTalesInventoryMainLeftUserWidget::InitializeOnePageData(TMultiMap<FName, FTalesInventoryItemSlot>& PageData,
	UWrapBox*& Box)
{
	if(Box)
	{
		Box->ClearChildren();
		if(PageData.IsEmpty())
		{
			UTalesSlotUserWidget* Widget = CreateWidget<UTalesSlotUserWidget>(this, SlotClass);
			Box->AddChildToWrapBox(Widget);
		}else
		{
			for(auto& item : PageData)
			{
				UTalesSlotUserWidget* Widget = CreateWidget<UTalesSlotUserWidget>(this, SlotClass);
				Widget->SetDataConstruct(item.Value);
				Box->AddChildToWrapBox(Widget);
			}
		}
	}
}

void UTalesInventoryMainLeftUserWidget::SetTalesCharacterOwner()
{
	auto Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	TalesCharacterOwner = Cast<ATalesCharacter>(Character);
	if(!IsValid(TalesCharacterOwner))
	{
		TalesCharacterOwner = nullptr;
		UE_LOG(LogTemp, Error, TEXT("Inventroy Main Left: Error to Get TalesCharacter."))
	}
}
