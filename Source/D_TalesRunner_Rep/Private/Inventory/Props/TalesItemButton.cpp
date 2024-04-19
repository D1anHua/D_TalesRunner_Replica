// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesItemButton.h"

UTalesItemButton::UTalesItemButton()
{
	OnClicked.AddDynamic(this, &ThisClass::OnClick);
	OnHovered.AddDynamic(this, &ThisClass::OnHover);
	OnUnhovered.AddDynamic(this, &ThisClass::OnUnHover);
}

void UTalesItemButton::OnClick()
{
	ClickDelegate.Broadcast(Name);
}

void UTalesItemButton::OnHover()
{
	HoverDelegate.Broadcast(Name);
}

void UTalesItemButton::OnUnHover()
{
	UnHoverDelegate.Broadcast(Name);
}
