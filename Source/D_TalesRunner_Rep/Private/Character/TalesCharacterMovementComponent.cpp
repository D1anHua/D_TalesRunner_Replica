
#include "Character/TalesCharacterMovementComponent.h"

#include "Character/TalesCharacter.h"
#include "Components/CapsuleComponent.h"
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
	MaxSprintSpeed = 1400;
	MaxWalkSpeed = 700;
}

void UTalesCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	TalesCharacterOwner = Cast<ATalesCharacter>(GetOwner());
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

bool UTalesCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMocementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMocementMode;
}

void UTalesCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}


void UTalesCharacterMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UTalesCharacterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}

// Slide
void UTalesCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if(deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	// Root Motion
	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if(!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(MinSlideSpeed, 2))
	{
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	// Surface Gravity
	Velocity += SlideGravityForce * FVector::DownVector * deltaTime;

	// Perform the move
	if(FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	// Calc Velocity
	if(!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(deltaTime, SlideFriction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	// Perform Move
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	// Main Function: We want to call to actually move the Character
	// Sweep is Scan when move, 使我们不会撞墙
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	// If Hit Something
	if(Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		// UseFul Function
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	FHitResult NewSurfaceHit;
	if(!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(MinSlideSpeed, 2))
	{
		ExitSlide();
	}

	// Update Outgoing Velocity & Acceleration
	if(!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
	
}

bool UTalesCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, TalesCharacterOwner->GetIgnoreCharacterParams());
}

void UTalesCharacterMovementComponent::EnterProne()
{
}

void UTalesCharacterMovementComponent::ExitProne()
{
}

void UTalesCharacterMovementComponent::PhysProne(float deltaTime, int32 Iterations)
{
	if(deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if(!CharacterOwner ||
		(
			!CharacterOwner->GetController() &&
			!bRunPhysicsWithNoController &&
			!HasAnimRootMotion() &&
			!CurrentRootMotion.HasOverrideVelocity() &&
			(CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	// Perform the move
	while(
		(remainingTime >= MIN_TICK_TIME) &&
		(Iterations < MaxSimulationIterations) &&
		CharacterOwner &&
		(
			CharacterOwner->GetController() ||
			bRunPhysicsWithNoController ||
			CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);

		// Save Current Values
		UPrimitiveComponent* const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != nullptr) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure Velocity is horizontal
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;
		Acceleration.Z = 0.f;

		// Apply Acceleration
		CalcVelocity(timeTick, GroundFriction, false, GetMaxBrakingDeceleration());

		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity;
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;

		if(bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			// Real movement 
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if(IsFalling())
			{
				// Pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if(DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));
				}
				StartNewPhysics(remainingTime, Iterations);
				return;
			}
			else if(IsSwimming()) // Just entered water
			{
				StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
				return;
			}
		}

		// Update floor
		// StepUp might have already done it for us.
	}
		
	
}

bool UTalesCharacterMovementComponent::GetProne(FHitResult& Hit) const
{
	return true;
}

void UTalesCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
                                                         const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(Safe_bWantToSprint)
		{
			MaxWalkSpeed = MaxSprintSpeed;
		}
		else
		{
			MaxWalkSpeed = MaxWalkSpeed;
		}
	}

	Safe_bPrevWantsToCrouch = bWantsToCrouch;
}

bool UTalesCharacterMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UTalesCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

void UTalesCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if(MovementMode == MOVE_Walking && !bWantsToCrouch && Safe_bPrevWantsToCrouch)
	{
		FHitResult PotentialSlideSurface;
		if(Velocity.SizeSquared() > pow(MinSlideSpeed, 2) && GetSlideSurface(PotentialSlideSurface))
		{
			EnterSlide();
		}
	}

	if(IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
	{
		ExitSlide();
	}
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UTalesCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	// Super::StartNewPhysics()
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
	}
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
	Saved_bPrevWantsToCrouch = CharacterMovement->Safe_bPrevWantsToCrouch;
}

void UTalesCharacterMovementComponent::FSavedmove_Tales::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UTalesCharacterMovementComponent* CharacterMovement = Cast<UTalesCharacterMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->Safe_bWantToSprint = Saved_bWantsToSprint;
	CharacterMovement->Safe_bPrevWantsToCrouch = Saved_bPrevWantsToCrouch;
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
