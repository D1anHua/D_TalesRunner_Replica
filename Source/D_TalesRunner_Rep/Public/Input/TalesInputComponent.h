// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/TalesInputConfig.h"
#include "TalesInputComponent.generated.h"

struct FGameplayTag;
class UEnhancedInputLocalPlayerSubsystem;
class UInputConfig;

/*!
 * UTalesInputComponent
 *
 * component used to manage input mappings and bindings using an input config data assets
 */
UCLASS(Config = Input)
class D_TALESRUNNER_REP_API UTalesInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	UTalesInputComponent(const FObjectInitializer& ObjectInitializer);
	
	template<class UserClass, typename FuncType>
	void BindNativeAction(const UTalesInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UTalesInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
void UTalesInputComponent::BindNativeAction(const UTalesInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if(const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UTalesInputComponent::BindAbilityActions(const UTalesInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	for(const FTalesInputAction& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			if(PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if(ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}

