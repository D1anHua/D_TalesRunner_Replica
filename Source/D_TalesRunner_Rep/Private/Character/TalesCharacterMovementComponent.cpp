
#include "Character/TalesCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "NativeGameplayTags.h"
#include "System/TalesGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_MovementStopped, "Gameplay.MovementStopped");

namespace TalesCharacter
{
	static float GroundTraceDistance = 100000.f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("TalesCharacter.GroundTraceDistance"), GroundTraceDistance, TEXT("Distance to trace down when generateing ground information."), ECVF_Cheat);
}

UTalesCharacterMovementComponent::UTalesCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UTalesCharacterMovementComponent::SimulateMovement(float DeltaTime)
{
	if(bHasReplicatedAcceleration)
	{
		// Preserve our replicated acceleration
		const FVector OriginalAcceleration = Acceleration;
		UCharacterMovementComponent::SimulateMovement(DeltaTime);
		Acceleration = OriginalAcceleration;
	}
	else
	{
		UCharacterMovementComponent::SimulateMovement(DeltaTime);
	}
}

const FTalesCharacterGroundInfo& UTalesCharacterMovementComponent::GetGroundInfo()
{
	if(!CharacterOwner || (GFrameCounter == CachedGroundInfo.LastUpdateFrame))
	{
		return CachedGroundInfo;
	}
	if(MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
		check(CapsuleComp);
		const float CapsuleHalfWeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - TalesCharacter::GroundTraceDistance - CapsuleHalfWeight));
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(UTalesCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParams;
		InitCollisionParams(QueryParams, ResponseParams);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParams);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = TalesCharacter::GroundTraceDistance;

		if(MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if(HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfWeight), 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;
	return CachedGroundInfo;
}

void UTalesCharacterMovementComponent::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	bHasReplicatedAcceleration = true;
	Acceleration = InAcceleration;
}

FRotator UTalesCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if(ASC->HasMatchingGameplayTag(TalesGameplayTags::TAG_Gameplay_MovementStopped))
		{
			return FRotator(0, 0, 0);
		}
	}
	return UCharacterMovementComponent::GetDeltaRotation(DeltaTime);
}

float UTalesCharacterMovementComponent::GetMaxSpeed() const
{
	if(UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		if(ASC->HasMatchingGameplayTag(TalesGameplayTags::TAG_Gameplay_MovementStopped))
		{
			return 0;
		}
	}
	return UCharacterMovementComponent::GetMaxSpeed();
}
