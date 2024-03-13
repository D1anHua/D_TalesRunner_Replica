// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/TalesPlayerState.h"
#include "GamePlay/TalesPlayerControllerBase.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"
#include "Net/UnrealNetwork.h"
#include "System/TalesLogChannels.h"

ATalesPlayerState::ATalesPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ATalesPlayerControllerBase* ATalesPlayerState::GetTalesPlayerController() const
{
	return Cast<ATalesPlayerControllerBase>(GetOwner());
}

void ATalesPlayerState::SetPawnData(const UTalesPawnData* InPawnData)
{
	check(InPawnData);
	if(GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if(PawnData)
	{
		UE_LOG(LogTales, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// TODO AbilitySubSystem
	ForceNetUpdate();
}

void ATalesPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ATalesPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ATalesPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void ATalesPlayerState::Reset()
{
	Super::Reset();

	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for(UPlayerStateComponent* Component : ModularComponents)
	{
		Component->Reset();
	}
}

void ATalesPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	TInlineComponentArray<UPlayerStateComponent*> PlayerStateComponents;
	GetComponents(PlayerStateComponents);
	for(UPlayerStateComponent* SourcePSComp : PlayerStateComponents)
	{
		if(UPlayerStateComponent* TargetComp = Cast<UPlayerStateComponent>(static_cast<UObject*>(FindObjectWithOuter(PlayerState, SourcePSComp->GetClass(), SourcePSComp->GetFName()))))
		{
			SourcePSComp->CopyProperties(TargetComp);
		}
	}
}

void ATalesPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams ShardParams;
	ShardParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, ShardParams);
}

void ATalesPlayerState::OnRep_PawnData()
{
}
