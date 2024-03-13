#pragma once

#include "CommonInputTypeEnum.h"
#include "GameplayTagContainer.h"
#include "PlayerMappableInputConfig.h"
#include "TalesMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT(BlueprintType)
struct FLoaderMappableConfigPair
{
	GENERATED_BODY()
	
	FLoaderMappableConfigPair() = default;
	FLoaderMappableConfigPair(const UPlayerMappableInputConfig* InConfig, ECommonInputType InType, const bool InIsActive)
		: Config(InConfig)
		, Type(InType)
		, bIsActive(InIsActive)
	{
	}

	//!!
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<const UPlayerMappableInputConfig> Config = nullptr;

	//!! TODO
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	//! TODO
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsActive = false;
};

USTRUCT()
struct FMappableConfigPair
{
	GENERATED_BODY()

	FMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	/*!
	 * The type of config that this is. Useful for filtering out configs by the current input device
	 * for things like the settings screen, or if you only want to apply this config when a certain
	 * input type is being used.
	 */
	UPROPERTY(EditAnywhere)
	ECommonInputType Type = ECommonInputType::Count;

	/*!
	 * Container of platform traits that must be set in order for this input to be activated.
	 * 
	 * If the platform does not have one of the traits specified it can still be registered, but cannot
	 * be activated. 
	 */
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DependentPlatformTraits;

	//! If the current platform has any of these traits, then this config will not be actived
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer ExcludedPlatformTraits;

	/*! If true, then this input config will be activated when it's associated Game Feature is activated.
	 * This is normally the desirable behavior
	 */
	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;

	/*! Returns true if this config pair can be activated based on the current platform traits and settings. */
	bool CanBeActivated() const;
};


