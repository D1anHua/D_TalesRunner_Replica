// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TalesGameInstance.generated.h"

class ATalesPlayerControllerBase;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTalesGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ATalesPlayerControllerBase* GetPrimaryPlayerController() const;

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	
};
