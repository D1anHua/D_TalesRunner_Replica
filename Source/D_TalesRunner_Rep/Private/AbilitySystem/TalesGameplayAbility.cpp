// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TalesGameplayAbility.h"
#include "AbilitySystem/TalesAbilitySystemCompBase.h"
#include "GamePlay/TalesPlayerControllerBase.h"
#include "AbilitySystemLog.h"
#include "Character/TalesCharacter.h"

UTalesGameplayAbility::UTalesGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UTalesGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayEffectContextHandle EffectContext = ActorInfo->AbilitySystemComponent->MakeEffectContext();
	for(auto GameplayEffect : OngoingEffectsToApplyOnStart)
	{
		if(!GameplayEffect.Get()) continue;
		if(auto AbilitySystem = GetTalesAbilitySystemComponentFromActorInfo())
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
			if(SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				if(!ActiveGEHandle.WasSuccessfullyApplied())
				{
					ABILITY_LOG(Log, TEXT("Ability %s failed to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
				}
			}	
		}
	}
	if(IsInstantiated())
	{
		for(auto GameplayEffect : OngoingEffectsToRemoveOnEnd)
		{
			if(!GameplayEffect.Get()) continue;
			if(auto AbilityComponent = GetTalesAbilitySystemComponentFromActorInfo())
			{
				FGameplayEffectSpecHandle SpecHandle = AbilityComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
				if(SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveGEHandle = AbilityComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					if(!ActiveGEHandle.WasSuccessfullyApplied())
					{
						ABILITY_LOG(Log, TEXT("Ability %s failed to apply startup effect %s"), *GetName(), *GetNameSafe(GameplayEffect));
					}
					else
					{
						RemoveOnEndEffectHandles.Add(ActiveGEHandle);
					}
				}
			}
		}
	}
}

void UTalesGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if(IsInstantiated())
	{
		for(FActiveGameplayEffectHandle ActiveEffectHandle : RemoveOnEndEffectHandles)
		{
			if(ActiveEffectHandle.IsValid())
			{
				ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffectHandle);
			}
		}

		RemoveOnEndEffectHandles.Empty();
	}
}

UTalesAbilitySystemCompBase* UTalesGameplayAbility::GetTalesAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UTalesAbilitySystemCompBase>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ATalesPlayerControllerBase* UTalesGameplayAbility::GetTalesPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ATalesPlayerControllerBase>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

ATalesCharacter* UTalesGameplayAbility::GetTalesCharacterFromActionInfo() const
{
	return Cast<ATalesCharacter>(GetAvatarActorFromActorInfo());
}

AController* UTalesGameplayAbility::GetControllerFromActorInfo() const
{
	if(CurrentActorInfo)
	{
		if(AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// look for a player controller or pawn in the owner chain
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while(TestActor)
		{
			if(AController* C = Cast<AController>(TestActor))
			{
				return C;
			}
			if(APawn * Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}
