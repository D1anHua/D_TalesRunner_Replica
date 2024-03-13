// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/TalesAbilitySet.h"
#include "AttributeSystem/TalesGameplayAbility.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "GameplayEffect.h"
#include "System/TalesLogChannels.h"

void FTalesAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if(Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FTalesAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if(Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FTalesAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FTalesAbilitySet_GrantedHandles::TakeFromAbilitySystem(UTalesAbilitySystemCompBase* TalesASC)
{
	check(TalesASC);

	if(!TalesASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets
		return;
	}

	for(const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if(Handle.IsValid())
		{
			TalesASC->ClearAbility(Handle);
		}
	}

	for(const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if(Handle.IsValid())
		{
			TalesASC->RemoveActiveGameplayEffect(Handle);
		}
	}
	for(UAttributeSet* Set : GrantedAttributeSets)
	{
		TalesASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UTalesAbilitySet::UTalesAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTalesAbilitySet::GiveToAbilitySystem(UTalesAbilitySystemCompBase* TalesASC,
	FTalesAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(TalesASC);

	if(!TalesASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Grant the gameplay abilities
	for(const FTalesAbilitySet_GameplayAbility& AbilityToGrant : GrantedGameplayAbilities)
	{
		if(!IsValid(AbilityToGrant.Ability))
		{
			// UE_LOG(LogTalesAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}
		UTalesGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UTalesGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = TalesASC->GiveAbility(AbilitySpec);
		if(OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects
	for(const FTalesAbilitySet_GameplayEffect& EffectToGrant : GrantedGameplayEffects)
	{
		if(!IsValid(EffectToGrant.GameplayEffect))
		{
			// UE_LOG(LogTalesAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GamePlayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = TalesASC->ApplyGameplayEffectToSelf(GamePlayEffect, EffectToGrant.EffectLevel, TalesASC->MakeEffectContext());
		if(OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets
	for(const FTalesAbilitySet_AttributeSet& SetToGrant : GrantedAttributes)
	{
		if(!IsValid(SetToGrant.AttributeSet))
		{
			// UE_LOG(LogTalesAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(TalesASC->GetOwner(), SetToGrant.AttributeSet);
		TalesASC->AddAttributeSetSubobject(NewSet);

		if(OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}

