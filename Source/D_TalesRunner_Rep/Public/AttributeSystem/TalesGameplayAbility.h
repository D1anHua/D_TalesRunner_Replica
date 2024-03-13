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

	UFUNCTION(BlueprintCallable, Category = "Tales|Ability")
	UTalesAbilitySystemCompBase* GetTalesAbilityStstemComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category = "Tales|Ability")
	ATalesPlayerControllerBase* GetTalesPlayerControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category = "Tales|Ability")
	AController* GetControllerFromActorInfo() const;

	ETalesAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; };
	ETalesAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; };

protected:
	// TODO: 暂时不用
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;
	//! 不设置蓝图实现版本: BlueprintImplementEvent

	//! UGameplayAbility Interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;

protected:
	//! Defines how this ability is meant to activate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tales|Ability Activation")
	ETalesAbilityActivationPolicy ActivationPolicy;

	//! Defines the relationship between this ability activating and other ability activating
	ETalesAbilityActivationGroup ActivationGroup;

	// TODO: 不考虑 Additional Cost(暂时没有需求)

	// TODO: Failure tags 暂时也没有考虑
	//! Map of failure tags to simple error messages
	TMap<FGameplayTag, FText> FailureTagToUserFacingMessages;

	// TODO: Camera Mode 现在还没有, 后面可能需要	
	
};
