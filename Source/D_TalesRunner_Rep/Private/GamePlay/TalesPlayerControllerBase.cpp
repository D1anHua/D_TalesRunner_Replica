// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/TalesPlayerControllerBase.h"
#include "Components/GameFrameworkComponentManager.h"

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
