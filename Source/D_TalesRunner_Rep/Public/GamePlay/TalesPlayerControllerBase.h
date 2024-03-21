// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TalesPlayerControllerBase.generated.h"

class ATalesPlayerState;
/**
 * Default Player Controller, 为PlayerController创建一个基类
 */
UCLASS()
class D_TALESRUNNER_REP_API ATalesPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	ATalesPlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Tales|PlayerController")
	ATalesPlayerState* GetTalesPlayerState() const;
	
	//!  Begin AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//!	 End AActor Interface
};
