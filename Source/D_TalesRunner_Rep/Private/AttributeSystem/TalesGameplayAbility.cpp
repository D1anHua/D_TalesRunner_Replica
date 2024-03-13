// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSystem/TalesGameplayAbility.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "GamePlay/TalesPlayerControllerBase.h"
#include "AttributeSystem/TalesAbilitySimpleFailureMessage.h"

UTalesGameplayAbility::UTalesGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

UTalesAbilitySystemCompBase* UTalesGameplayAbility::GetTalesAbilityStstemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UTalesAbilitySystemCompBase>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ATalesPlayerControllerBase* UTalesGameplayAbility::GetTalesPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ATalesPlayerControllerBase>(CurrentActorInfo->PlayerController.Get()) : nullptr);
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

void UTalesGameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
	bool bSimpleFailureFound = false;
	for(FGameplayTag Reason : FailedReason)
	{
		if(!bSimpleFailureFound)
		{
			if(const FText* pUserFacingMessage = FailureTagToUserFacingMessages.Find(Reason))
			{
				FTalesAbilitySimpleFailureMessage Message;
				Message.PlayerController = GetActorInfo().PlayerController.Get();
				Message.FailureTags = FailedReason;
				Message.UserFacingReason = *pUserFacingMessage;
			}
		}
	}
}

bool UTalesGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if(!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}
	// TODO:
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UTalesGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
	// The ability can not block canceling if it's replaceable ??
	if(!bCanBeCanceled && (ActivationGroup == ETalesAbilityActivationGroup::Exclusive_Replaceable))
	{
		UE_LOG(LogTemp, Error, TEXT("Set can be canceled: Ability [%s] can not block canceling because its activation group is replaceable,"), *GetName());
		return;
	}
	Super::SetCanBeCanceled(bCanBeCanceled);
}
