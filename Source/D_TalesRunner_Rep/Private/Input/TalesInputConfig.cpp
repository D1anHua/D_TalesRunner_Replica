// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/TalesInputConfig.h"
#include "System/TalesLogChannels.h"

UTalesInputConfig::UTalesInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UTalesInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(const FTalesInputAction& Action : NativeInputActions)
	{
		if(Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTales, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}

const UInputAction* UTalesInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for(const FTalesInputAction& Action : AbilityInputActions)
	{
		if(Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;	
		}
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTales, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
