// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "System/TalesRunnerTypes.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
};
