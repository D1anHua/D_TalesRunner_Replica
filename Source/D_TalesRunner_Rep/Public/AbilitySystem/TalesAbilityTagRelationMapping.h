// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "TalesAbilityTagRelationMapping.generated.h"


//! Struct that defines the reletionship between different ability tag
USTRUCT()
struct FTalesAbilityTagRelationship
{
	GENERATED_BODY()

	//! The tag that the container relationship is about, Single, but abilites can have multiple of thest
	UPROPERTY(EditAnywhere, Category = Ability, meta = (Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	//! The other ability tags that will be blocked by any ability using this tag
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	//! The other ability tags that will be canceled by any ability using this tag
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	//! If an ability has the tag, this is implicitly added to the activation required tags of the ability
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	//! If an ability has the tag, this is implicitly added to the activation blocked tags of the ability
	FGameplayTagContainer ActivationBlockedTags;
};


/**
 * Mapping of how ability tags block or cancel other abilities
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesAbilityTagRelationMapping : public UDataAsset
{
	GENERATED_BODY()

private:
	//! The list of relationships between different gameplay tags
	UPROPERTY(EditAnywhere, Category = Ability, meta = (TitleProperty = "AbilityTag"))
	TArray<FTalesAbilityTagRelationship> AbilityTagRelationships;
	
public:
	//! Given a set of ability tags, parse the tag relationship and fill out tags to block and cancel
	void GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutTagsToBlock, FGameplayTagContainer* OutTagsToCancel) const;

	//! Given a set of ability tags, add additional required and blocking tags
	void GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutAbilitionRequired, FGameplayTagContainer* OutActivationBlocked) const;

	//! Returns true if the specified ability tags are canceled by the passed in action tag
	bool IsAbilityCancelledByTag(const FGameplayTagContainer& AbilityTags, const FGameplayTag& ActionTag);
};
