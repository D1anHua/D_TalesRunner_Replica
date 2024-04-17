// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TalesCharacterAnimInstance.h"

#include "Character/TalesCharacter.h"
#include "Character/TalesCharacterMovementComponent.h"

void UTalesCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	TalesCharacter = Cast<ATalesCharacter>(TryGetPawnOwner());
	if(TalesCharacter)
	{
		TalesCharacterMovementComponent = TalesCharacter->GetTalesCharacterMovement();
	}
}

void UTalesCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!TalesCharacter || !TalesCharacterMovementComponent) return;

	GetGroundSpeed();
	GetAirSpeed();
	GetShouldMove();
	GetIsFalling();
	GetCustomMode();
	GetVelocityComponent();
}

void UTalesCharacterAnimInstance::GetGroundSpeed()
{
	Velocity = TalesCharacter->GetVelocity();
	GroundSpeed = Velocity.Length();
}

void UTalesCharacterAnimInstance::GetAirSpeed()
{
	AirSpeed = TalesCharacter->GetVelocity().Z;
}

void UTalesCharacterAnimInstance::GetShouldMove()
{
	bShouldMove = TalesCharacterMovementComponent->GetCurrentAcceleration().Size() > 0 && GroundSpeed > 5.f && !TalesCharacterMovementComponent->IsFalling();
}

void UTalesCharacterAnimInstance::GetIsFalling()
{
	bIsFalling = TalesCharacterMovementComponent->IsFalling();
}

void UTalesCharacterAnimInstance::GetCustomMode()
{
	bIsSliding   = TalesCharacterMovementComponent->IsSlide();
	bIsCrouching = TalesCharacterMovementComponent->IsCrouching();	
	bIsProning   = TalesCharacterMovementComponent->IsProne();	
	bIsClimbing  = TalesCharacterMovementComponent->IsClimbing();	
}


void UTalesCharacterAnimInstance::GetVelocityComponent()
{
	FVector TempV = TalesCharacterMovementComponent->GetUnRotatedClimbVelocity();
	VerticalVelocity = TalesCharacterMovementComponent->GetUnRotatedClimbVelocity().Z;
	HorizontalVelocity = TalesCharacterMovementComponent->GetUnRotatedClimbVelocity().Y;
}
