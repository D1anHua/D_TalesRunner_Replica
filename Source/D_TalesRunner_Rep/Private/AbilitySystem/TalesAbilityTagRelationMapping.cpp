// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TalesAbilityTagRelationMapping.h"

void UTalesAbilityTagRelationMapping::GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& AbilityTags,
                                                                     FGameplayTagContainer* OutTagsToBlock, FGameplayTagContainer* OutTagsToCancel) const
{
	for(const FTalesAbilityTagRelationship& Tag : AbilityTagRelationships)
	{
		if(AbilityTags.HasTag(Tag.AbilityTag))
		{
			if(OutTagsToBlock)
			{
				OutTagsToBlock->AppendTags(Tag.AbilityTagsToBlock);
			}
			if(OutTagsToCancel)
			{
				OutTagsToCancel->AppendTags(Tag.AbilityTagsToCancel);
			}
		}	
	}
}

void UTalesAbilityTagRelationMapping::GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& AbilityTags,
	FGameplayTagContainer* OutActionRequired, FGameplayTagContainer* OutActivationBlocked) const
{
	for(const FTalesAbilityTagRelationship& Tag : AbilityTagRelationships)
	{
		if(AbilityTags.HasTag(Tag.AbilityTag))
		{
			if(OutActionRequired)
			{
				OutActionRequired->AppendTags(Tag.ActivationRequiredTags);
			}
			if(OutActivationBlocked)
			{
				OutActivationBlocked->AppendTags(Tag.ActivationBlockedTags);
			}
		}
	}
}

bool UTalesAbilityTagRelationMapping::IsAbilityCancelledByTag(const FGameplayTagContainer& AbilityTags,
	const FGameplayTag& ActionTag)
{
	for(const FTalesAbilityTagRelationship& Tag : AbilityTagRelationships)
	{
		if(Tag.AbilityTag == ActionTag && Tag.AbilityTagsToCancel.HasAny(AbilityTags))
		{
			return true;
		}
	}
	return false;
}
