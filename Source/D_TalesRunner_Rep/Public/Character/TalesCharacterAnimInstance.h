// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TalesCharacterAnimInstance.generated.h"


class UTalesCharacterMovementComponent;
class ATalesCharacter;

/**
 * TalesCharacterAnimInstance
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()  ATalesCharacter* TalesCharacter;
	UPROPERTY()  UTalesCharacterMovementComponent* TalesCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float AirSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float HorizontalVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	float VerticalVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsSliding;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsProning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = reference, meta = (AllowPrivateAccess = "true"))
	bool bIsClimbing;

	void GetGroundSpeed();
	void GetAirSpeed();
	void GetShouldMove();
	void GetIsFalling();
	void GetCustomMode();
	void GetVelocityComponent();
};
