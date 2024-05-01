// Fill out your copyright notice in the Description page of Project Settings.


#include "System/TalesGameInstance.h"
#include "Engine/GameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "AbilitySystemGlobals.h"

#include "GamePlay/TalesPlayerControllerBase.h"
#include "System/TalesGameplayTags.h"

UTalesGameInstance::UTalesGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}



ATalesPlayerControllerBase* UTalesGameInstance::GetPrimaryPlayerController() const
{
	return Cast<ATalesPlayerControllerBase>(Super::GetPrimaryPlayerController());
}

void UTalesGameInstance::Init()
{
	Super::Init();
	// GAS Course
	UAbilitySystemGlobals::Get().InitGlobalData();

	
	// Register our custom init states
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if(ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(TalesGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(TalesGameplayTags::InitState_DataAvailable, false, TalesGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(TalesGameplayTags::InitState_DataInitialized, false, TalesGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(TalesGameplayTags::InitState_GameplayReady, false, TalesGameplayTags::InitState_DataInitialized);
	}
}

void UTalesGameInstance::Shutdown()
{
	Super::Shutdown();
}
