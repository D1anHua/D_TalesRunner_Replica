#pragma once

#include "CoreMinimal.h"
#include "TalesRunnerTypes.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;

	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	// class UCharacterAnimDataAsset* CharacterAnimDataAsset;
};