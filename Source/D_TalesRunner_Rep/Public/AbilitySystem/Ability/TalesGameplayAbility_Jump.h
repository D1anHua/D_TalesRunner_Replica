// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/TalesGameplayAbility.h"
#include "TalesGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesGameplayAbility_Jump : public UTalesGameplayAbility
{
	GENERATED_BODY()

	UTalesGameplayAbility_Jump();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
