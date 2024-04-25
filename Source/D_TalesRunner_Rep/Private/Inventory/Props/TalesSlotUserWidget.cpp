// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesSlotUserWidget.h"

#include "Character/TalesCharacter.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Inventory/TalesInventoryComponent.h"
#include "Inventory/TalesInventoryMainLeftUserWidget.h"
#include "Inventory/TalesInventoryUserWidget.h"
#include "Inventory/Props/TalesHealthBarUserWidget.h"
#include "Inventory/Props/TalesInfoCueUserWidget.h"
#include "Inventory/Props/TalesInventoryUWActionMenu.h"
#include "Kismet/GameplayStatics.h"

void UTalesSlotUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(SlotButton)
	{
		SlotButton->OnClicked.AddDynamic(this, &ThisClass::OnClick);
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

FReply UTalesSlotUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// if(InMouseEvent.IsMouseButtonDown(FKey(FName("RightMouseButton"))))
	// {
	// }
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


void UTalesSlotUserWidget::OnClick()
{
	if(Data.Quantity != 0)
	{
		if(ensureAlways(ActionMenuClass))
		{
			auto ActionMenu = CreateWidget<UTalesInventoryUWActionMenu>(GetWorld(), ActionMenuClass);
			if(Data.ItemType == Eatable)
			{
				ActionMenu->SetText(FText::FromString("Eat"));
			}
			ActionMenu->SetInstigatorActor(this);
			ActionMenu->AddToViewport();
		}
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

	// Get Source Power
	
	InfoCueWidget->InitializeAllInfo(GetSourcePower(), Data);
	if(IsActivate()) InfoCueWidget->SetActivate();
	InfoCueWidget->SetVisibility(ESlateVisibility::Visible);
}

void UTalesSlotUserWidget::OnUnHovered()
{
	HoverBorderImage->SetVisibility(ESlateVisibility::Hidden);	
	SlotSelectBorder->SetBrushFromTexture(UnHoverBorder);
	SlotSelectBorder->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.4f));
	auto InfoCueWidget = InventoryComponent->GetInventoryUserWidget()->GetInfoCueUserWidget();
	if(IsActivate()) InfoCueWidget->SetUnActivate();
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
	UnActivateSlot();
}

void UTalesSlotUserWidget::ActivateSlot()
{
	this->EquipBackgroundImage->SetVisibility(ESlateVisibility::Visible);
}

void UTalesSlotUserWidget::UnActivateSlot()
{
	this->EquipBackgroundImage->SetVisibility(ESlateVisibility::Hidden);
}

void UTalesSlotUserWidget::UseThisItem()
{
	// 主要又三步:
	// 1. 要获得该Item的效果
	// 2. 减少该Item的数量(在当前的widget中)
	// 3. 减少Component中的数目
	if(Data.Quantity <= 0){return;}
	switch (Data.ItemType)
	{
	case Sward:
		InventoryComponent->SetSwardSlot(this->Data);
		InventoryComponent->GetInventoryUserWidget()->GetMainLeftMenu()->SetSwardSlotOnActivate(this);
		break;
	case Shield:
		InventoryComponent->SetShieldSlot(this->Data);
		InventoryComponent->GetInventoryUserWidget()->GetMainLeftMenu()->SetShieldSlotOnActivate(this);
		break;
	case Eatable:
		UseFood();
		break;
	default:
		break;
	}
}

void UTalesSlotUserWidget::UseFood()
{
	// 在更新显示数据之前就应该更新实际数据
	// 更新实际数据
	InventoryComponent->PackageDataDecrease(Data , -1);
	// 更新该widget的数量
	int ItemNum = Data.Quantity - 1;
	if(ItemNum > 0)
	{
		// 更新数量
		ItemNumber->SetText(FText::FromString(FString::FromInt(ItemNum)));
		// 更新这个表里面保存的暂时的变量
		Data.Quantity -= 1;
	}
	else
	{
		RemoveFromParent();		
	}
	
	// 实际上获得该食物的补血量
	if(Data.ItemType == Eatable)
	{
		float heartNow = InventoryComponent->GetHeartNow();
		
		FTalesInventoryMenuItem* Row = Data.ItemRowHandle.GetRow<FTalesInventoryMenuItem>(FString("Searching for Textrue"));
		if(Row)
		{
			heartNow += Row->Power;
			InventoryComponent->GetInventoryUserWidget()->GetHealthBarUserWidget()->AddHelathRuntime(Row->Power);
		}
		InventoryComponent->SetHeartNow(heartNow);
	}
}

void UTalesSlotUserWidget::DropThisItem()
{
	auto RemoveSward = [&]()
	{
		InventoryComponent->PackageDataDecrease(Data, 0);
		if(IsActivate())
		{
			InventoryComponent->SetSwardSlot(FTalesInventoryItemSlot());
		}
		RemoveFromParent();
	};
	
	auto RemoveShield = [&]()
	{
		InventoryComponent->PackageDataDecrease(Data, 0);
		if(IsActivate())
		{
			InventoryComponent->SetShieldSlot(FTalesInventoryItemSlot());
		}
		RemoveFromParent();
	};

	auto RemoveEatable = [&]()
	{
		// 1. 必须 更新实际数据
		InventoryComponent->PackageDataDecrease(Data, -1);
		// 2. 从当前栏目中删除一个元素, 并更新Component中保存的数目
		// 更新该widget的数量
		int ItemNum = Data.Quantity - 1;
		if(ItemNum > 0)
		{
			// 更新数量
			ItemNumber->SetText(FText::FromString(FString::FromInt(ItemNum)));
			// 更新这个表里面保存的暂时的变量
			Data.Quantity -= 1;
		}
		else
		{
			RemoveFromParent();		
		}
	};
	// Drop操作:
	// 两部分
	// 1. 若是sward 或者 shield
	// 直接删除即可
	// 2. 若是食物
	// 正常判断
	switch (Data.ItemType)
	{
	case Sward:
		RemoveSward();
		break;
	case Shield:
		RemoveShield();
		break;
		
	case Eatable:
		RemoveEatable();
	default:
		break;
	}

	// 2. Spawn Item
	if(ensureAlways(ItemToBind))
	{
		auto Transform = InventoryComponent->GetOwner()->GetActorTransform();
		FVector RandomLoaction = FVector(1.f, 1.f, 0.f) * FMath::RandRange(-50.f, 50.f);
		Transform.SetLocation(Transform.GetLocation() + RandomLoaction);
		FActorSpawnParameters Parameters = FActorSpawnParameters();
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		auto Actor = GetWorld()->SpawnActor<ATalesInventoryItem>(ItemToBind, Transform, Parameters);
		auto NewData = this->Data;
		NewData.Quantity = 1;
		Actor->SetItemSlotOnInitialize(NewData);
	}
}

void UTalesSlotUserWidget::GetInventoryComponent()
{
	auto Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	OwningCharacter = Cast<ATalesCharacter>(Character);
	if(OwningCharacter)
	{
		InventoryComponent = OwningCharacter->GetTalesInventoryComponent();
		if(!IsValid(InventoryComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("Error in Slot User Widget: Unable to Get Inventory Componet"));
		}
	}	
}

bool UTalesSlotUserWidget::IsActivate()
{
	UTalesSlotUserWidget* ActivateSlot;
	switch (Data.ItemType)
	{
	case Sward:
		ActivateSlot = InventoryComponent->GetInventoryUserWidget()->GetMainLeftMenu()->GetSwardSlotOnActivate();
		break;
	case Shield:
		ActivateSlot = InventoryComponent->GetInventoryUserWidget()->GetMainLeftMenu()->GetShieldSlotOnActivate();
		break;
	default:
		return false;
	}

	if(ActivateSlot == this)
	{
		return true;
	}
	return false;
}

int UTalesSlotUserWidget::GetSourcePower()
{
	int SourcePower = 0;
	if(InventoryComponent)
	{
		FTalesInventoryMenuItem* Row;
		switch (Data.ItemType)
		{
		case Sward:
			if(InventoryComponent->GetSwardSlot().IsValid())
			{
				Row = InventoryComponent->GetSwardSlot().GetRow();
			}
			else
			{
				return 0;
			}
			break;
		case Shield:
			if(InventoryComponent->GetShieldSlot().IsValid())
			{
				Row = InventoryComponent->GetShieldSlot().GetRow();
			}
			else
			{
				return 0;
			}
			break;
		default:
			return 0;
			break;
		}
		if(Row)
		{
			SourcePower = Row->Power;
		}
	}
	return SourcePower;
}