// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesHeartUserWidget.h"

#include "Components/Image.h"

void UTalesHeartUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if(HeartImage)
	{
		SetImage(HeartNum);	
	}
}

void UTalesHeartUserWidget::SetImage(float HeartSize)
{
	this->HeartNum = HeartSize;
	if(HeartImage)
	{
		if(HeartNum > 0.98f)
		{
			HeartImage->SetBrushFromTexture(HeartFull);	
		}
		else if(HeartNum > 0.48f)
		{
			HeartImage->SetBrushFromTexture(HeartHalfFull);
		}else
		{
			HeartImage->SetBrushFromTexture(HeartEmpty);
		}
	}
}
