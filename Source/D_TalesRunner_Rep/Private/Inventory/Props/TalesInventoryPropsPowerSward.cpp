// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesInventoryPropsPowerSward.h"

#include "Components/TextBlock.h"

void UTalesInventoryPropsPowerSward::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTalesInventoryPropsPowerSward::SetPower(int Source, int Target)
{
	if(TargetPower && SourcePower)
	{
		TargetPower->SetText(FText::FromString(FString::FromInt(Target)));
		SourcePower->SetText(FText::FromString(FString::FromInt(Source)));
	}
}
