// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TalesInputConfig.generated.h"

/*!
 * FTalesInputAction
 *
 * Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FTalesInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/*!
 * UTalesInputConfig
 *
 * Non-Mutable data asset that contains input configuration properties
 */
UCLASS(BlueprintType, Const)
class UTalesInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UTalesInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Tales | Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Tales | Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	//! List of input actions used by the owner, These input actions are mapped to a gameplay tag and must be mannually bound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FTalesInputAction> NativeInputActions;

	//! List of input actions used by the owner, These input actions are mapped to a gameplay tag and are automatically bound to abilitied with matching input tags
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FTalesInputAction> AbilityInputActions;
	
};
