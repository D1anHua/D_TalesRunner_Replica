// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/TalesPlayerControllerBase.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GamePlay/TalesPlayerState.h"

ATalesPlayerControllerBase::ATalesPlayerControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ATalesPlayerState* ATalesPlayerControllerBase::GetTalesPlayerState() const
{
	return CastChecked<ATalesPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void ATalesPlayerControllerBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ATalesPlayerControllerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
