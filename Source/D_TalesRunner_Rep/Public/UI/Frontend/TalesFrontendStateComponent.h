// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "LoadingProcessInterface.h"
#include "Components/GameStateComponent.h"
#include "TalesFrontendStateComponent.generated.h"

enum class ECommonUserOnlineContext : uint8;
enum class ECommonUserPrivilege : uint8;
class UCommonPlayerInfo;
class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesFrontendStateComponent : public UGameStateComponent //, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	UTalesFrontendStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//! UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//! End of UActorComponent interface
	
private:
};
