// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/TalesStaminaSet.h"

#include "Net/UnrealNetwork.h"

UTalesStaminaSet::UTalesStaminaSet()
	: CurrentStamina(100.f)
	, MaxStamina(100.f)
{
	bOutOfHealth = false;
}


void UTalesStaminaSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTalesStaminaSet, CurrentStamina, COND_None, REPNOTIFY_Always);
}

void UTalesStaminaSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTalesStaminaSet, CurrentStamina, OldValue);
}

void UTalesStaminaSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UTalesStaminaSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttribute(Attribute, NewValue);
}

void UTalesStaminaSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if(Attribute == GetCurrentStaminaAttribute())
	{
		// Do not allow health to go negative or above max health
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
}
