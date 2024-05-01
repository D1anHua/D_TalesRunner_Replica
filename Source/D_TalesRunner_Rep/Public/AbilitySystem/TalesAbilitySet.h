// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "TalesAbilitySet.generated.h"

class UTalesAbilitySystemCompBase;
class UGameplayAbility;
class UTalesGameplayAbility;

/**
 * FTalesAbilitySet_GameplayAbility
 *
 * Data Used by the ability set to grant gameplay abilities
 */
USTRUCT(BlueprintType)
struct FTalesAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	//! Gameplay Ability to grant(准许)
	TSubclassOf<UTalesGameplayAbility> Ability = nullptr;

	//! Level of ability to grant
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	//! Tag Used to process input for the ability
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


/**
 * FTalesAbilitySet_GameplayEffect
 *
 * Data Used by the ability set to grant gameplay effects
 */
USTRUCT(BlueprintType)
struct FTalesAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	//! Gameplay effect to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;
	
	//! Level of Effect to grant
	UPROPERTY(EditDefaultsOnly)
	int32 EffectLevel = 1;
};


/**
 * FTalesAbilitySet_AttributeSet
 *
 * Data Used by the ability set to grant attribute sets
 */
USTRUCT(BlueprintType)
struct FTalesAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay Effect to grant
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

/**
 * FTalesAbilitySet_GrantedHandles
 *
 * Data Used to store handles to what has been granted by the ability set
 */
USTRUCT(BlueprintType)
struct FTalesAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UTalesAbilitySystemCompBase* TalesASC);

protected:
	//! Handles to the granted ability
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	//! Handles to the granted gameplay effects
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	//! Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 * UTalesAbilitySet
 *
 * Not-mutable data asset used to grant gameplay abilities and gameplay effects
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTalesAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(UTalesAbilitySystemCompBase* TalesASC, FTalesAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	//! Gameplay ablility to grant when this ability set is granted
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FTalesAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	//! Gameplay Effect to grant when this ability set is granted
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FTalesAbilitySet_GameplayEffect> GrantedGameplayEffects;

	//! Attribute Sets to grant when this ability set is granted
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FTalesAbilitySet_AttributeSet> GrantedAttributes;
};
