// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSystem/TalesAttributeSetBase.h"
#include "TalesStaminaSet.generated.h"

/**
 *  UTalesStaminaSet
 *
 *  Class that defines attributes that are necessary for consumes stamina
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesStaminaSet : public UTalesAttributeSetBase
{
	GENERATED_BODY()

public:
	UTalesStaminaSet();

	TALES_ATTRIBUTE_ACCESSORS(UTalesStaminaSet, CurrentStamina);
	TALES_ATTRIBUTE_ACCESSORS(UTalesStaminaSet, MaxStamina);
	TALES_ATTRIBUTE_ACCESSORS(UTalesStaminaSet, Healing);
	TALES_ATTRIBUTE_ACCESSORS(UTalesStaminaSet, Consume);

	//! Deletage to broadcast then the Stamina attribute reaches zero.
	mutable TalesAttributeEvent OnOutOfStamina;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes|Stamina", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData CurrentStamina;	

	UPROPERTY(EditDefaultsOnly, Category = "Attributes|Stamina")
	FGameplayAttributeData MaxStamina;

	//! Used to track when the Stamina reaches 0
	bool bOutOfHealth;

	//! Incoming Stamina, This is mapped directly to +Stamina
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
	//! Consume Stamina, This is mapped directily to -Stamina
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stamina", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Consume;
};
