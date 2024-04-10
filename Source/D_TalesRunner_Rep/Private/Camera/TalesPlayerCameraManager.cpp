// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/TalesPlayerCameraManager.h"

#include "Character/TalesCharacter.h"
#include "Character/TalesCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


ATalesPlayerCameraManager::ATalesPlayerCameraManager()
{
	CrouchBlendTime = 0.f;
	ViewPitchMax = 80.f;
	ViewPitchMax = -30.f;
}

void ATalesPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if(ATalesCharacter* TalesCharacter = Cast<ATalesCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UTalesCharacterMovementComponent* TCMC = TalesCharacter->GetTalesCharacterMovement();
		FVector TargetCrouchOffset = FVector(0, 0, TCMC->GetCrouchedHalfHeight() - TalesCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if(TCMC->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		// if(TCMC->IsMovingOnGround())
		// {
			OutVT.POV.Location += Offset;
		// }
	}
}
