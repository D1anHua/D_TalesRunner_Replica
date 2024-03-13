// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TalesAttributeSetBase.generated.h"

struct FGameplayEffectSpce;


/*!
 * @brief  This macro defines a set of helper functions for accessing and inti attributes
 *
 * The following example of the macro:
 *		ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health) will create the following functions:
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define TALES_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//Delegate used to broadcast attribute events
DECLARE_MULTICAST_DELEGATE_FourParams(TalesAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpce& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 *  UTalesAttributeSetBase
 *
 *  Base attribute set class for this project
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:

	UTalesAttributeSetBase() = default;

	//! Don't know the purpose of the overload
	virtual UWorld* GetWorld() const override;
};
