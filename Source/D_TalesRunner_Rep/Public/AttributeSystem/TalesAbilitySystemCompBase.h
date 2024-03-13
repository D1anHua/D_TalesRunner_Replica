// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TalesAbilitySystemCompBase.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesAbilitySystemCompBase : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTalesAbilitySystemCompBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
