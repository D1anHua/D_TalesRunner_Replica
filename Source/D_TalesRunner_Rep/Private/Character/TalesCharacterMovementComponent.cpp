
#include "Character/TalesCharacterMovementComponent.h"

#include "GameFramework/Character.h"

// UTalesCharacterMovementComponent::FSavedmove_Tales::FSavedmove_Tales()
// {
// 	FSavedMove_Character();
// 	Saved_bWantsToSprint = 0;
// }

UTalesCharacterMovementComponent::UTalesCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;

	// Sprint
	Safe_bWantToSprint = false;
	Sprint_MaxWalkSpeed = 1400;
	Walk_MaxWalkSpeed = 700;
	
}

void UTalesCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UTalesCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(Safe_bWantToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

void UTalesCharacterMovementComponent::SprintPressed()
{
	Safe_bWantToSprint = true;
}

void UTalesCharacterMovementComponent::SprintReleased()
{
	Safe_bWantToSprint = false;
}

void UTalesCharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;
}

FNetworkPredictionData_Client* UTalesCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);
	if(ClientPredictionData == nullptr)
	{
		UTalesCharacterMovementComponent* MutableThis = const_cast<UTalesCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Tales(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}	
	return ClientPredictionData;
}

// /--------------------------------  Saved move_Tales   -----------------------------

bool UTalesCharacterMovementComponent::FSavedmove_Tales::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                        ACharacter* InCharacter, float MaxDelta) const
{
	FSavedmove_Tales* NewTalesMove = static_cast<FSavedmove_Tales*>(NewMove.Get());
	if(Saved_bWantsToSprint != NewTalesMove->Saved_bWantsToSprint)
	{
		return false;
	}
	
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UTalesCharacterMovementComponent::FSavedmove_Tales::Clear()
{
	FSavedMove_Character::Clear();
	Saved_bWantsToSprint = 0;
}

uint8 UTalesCharacterMovementComponent::FSavedmove_Tales::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if(Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}

void UTalesCharacterMovementComponent::FSavedmove_Tales::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	UTalesCharacterMovementComponent* CharacterMovement = Cast<UTalesCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovement->Safe_bWantToSprint;	
}

void UTalesCharacterMovementComponent::FSavedmove_Tales::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UTalesCharacterMovementComponent* CharacterMovement = Cast<UTalesCharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->Safe_bWantToSprint = Saved_bWantsToSprint;
}

UTalesCharacterMovementComponent::FNetworkPredictionData_Client_Tales::FNetworkPredictionData_Client_Tales(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UTalesCharacterMovementComponent::FNetworkPredictionData_Client_Tales::AllocateNewMove()
{
	Super::AllocateNewMove();
	return FSavedMovePtr(new FSavedmove_Tales);
}
