// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TalesGameplayAbility.generated.h"

class UTalesAbilitySystemCompBase;
class ATalesPlayerControllerBase;

/*!
 *  ETalesAbilityActivationPolicy
 *
 *  Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class ETalesAbilityActivationPolicy : uint8
{
	//! Try to activate the ability when the input is triggered
	OnInputTriggered,
	//! Continually try to activate the ability while the input is active;
	WhileInputActive,
	//! Try to activate the ability when an avatar is assigned.
	OnSpawn
};

/*!
 * ETalesAbilityActivationGroup
 *
 * Defines how an ability activates in relation to other abilities.
 */
UENUM(BlueprintType)
enum class ETalesAbilityActivationGroup : uint8
{
	//! The ability doesn't block or replace other abilities. Most abilities should be set to this tag by default.
	Independent,
	//! The ability doesn't block other exclusive abilities but will be canceled if another Exclusive ability is activated.	
	Exclusive_Replaceable,
	//! While the ability is running, no other exclusive abilities may be activated.
	Exclusive_Blocking,

	MAX UMETA(Hidden)
};

//! Failure reason that can be used to play an animation montage when a failure occurs
USTRUCT(BlueprintType)
struct FTalesAbilityMontageFailureMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	//! All the reasons why this ability has failed
	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAnimMontage> FailureMontage = nullptr;
};


/**
 *  UTalesGameplayAbility
 *
 *  The base gameplay ability class used by this project
 *  Main to add above struct processing proxy
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UTalesAbilitySystemCompBase;

public:
	UTalesGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	UTalesAbilitySystemCompBase* GetTalesAbilitySystemComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	ATalesPlayerControllerBase* GetTalesPlayerControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	class ATalesCharacter* GetTalesCharacterFromActionInfo() const;

protected:
	//! UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToApplyOnStart;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

};
