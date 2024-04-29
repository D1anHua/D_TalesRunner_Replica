// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TalesCharacterAnimInstance.generated.h"

UENUM(BlueprintType)
enum EAnimEnumCardinalDirection
{
	ForwardDirection,
	BackwardDirection,
	LeftDirection,
	RightDirection,
};

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
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()  ATalesCharacter* TalesCharacter;
	UPROPERTY()  UTalesCharacterMovementComponent* TalesCharacterMovementComponent;

	// Distance Matching
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Location, meta = (AllowPrivateAccess = "true"))
	float DisplacementSinceLastUpdate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Location, meta = (AllowPrivateAccess = "true"))
	float DisplacementSpeed ;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Location, meta = (AllowPrivateAccess = "true"))
	FVector WorldLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFirstUpdate = true;
	
	// Velocity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	FVector LocalVelocity2D;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	float AirSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	float HorizontalVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	float VerticalVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	bool bShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	float LocalVelocityDirectionAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	float LocalVelocityDirectionAngleWithOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAnimEnumCardinalDirection>  LocalVelocityDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VelocityData, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAnimEnumCardinalDirection> LocalVelocityDirectionNoOffset;

	// Acceleration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AccelerationData, meta = (AllowPrivateAccess = "true"))
	FVector LocalAcceleration2D;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AccelerationData, meta = (AllowPrivateAccess = "true"))
	FVector PivotDirection2D;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AccelerationData, meta = (AllowPrivateAccess = "true"))
	bool bHasAcceleration;
	
	// CharacterStateData
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsSliding;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsProning;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsClimbing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsOnGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsFallingJump;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	float TimeJumpToApex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	float GroundDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterStateData, meta = (AllowPrivateAccess = "true"))
	bool bIsIntoWall;

	// Rotation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	FRotator WorldRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	float YawDeltaSinceLastUpdate;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	// float AdditiveLeanAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rotation, meta = (AllowPrivateAccess = "true"))
	float YawDeltaSpeed;

	// Acceleration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LocomotionSMData, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAnimEnumCardinalDirection> StartDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LocomotionSMData, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAnimEnumCardinalDirection> PivotInitialDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LocomotionSMData, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EAnimEnumCardinalDirection> CardinalDirectionFromAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LocomotionSMData, meta = (AllowPrivateAccess = "true"))
	float LastPivotTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LocomotionSMData, meta = (AllowPrivateAccess = "true"))
	bool bIsMovingPerpendicularToInitialPivot;
	
	// Turn In Place
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TurnInPlace, meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;
	
	// Settings
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Setting, meta = (AllowPrivateAccess = "true"))
	float CardinalDirectionDeadZone = 10.f;
	
	void UpdateRotationData();
	void UpdateVelocityData();
	void UpdateAccelerationData();
	
	void GetGroundSpeed();
	void GetAirSpeed();
	void GetShouldMove();
	void GetJumpParams();
	void GetCustomMode();
	void GetVelocityComponent();
	void UpdateLocationData(float DeltaSeconds);
	void UpdateWallDetection();
	EAnimEnumCardinalDirection SelectCardinalDirectionfromAngle(float angle, float DeadZone, EAnimEnumCardinalDirection CurrentDirection, bool bUseCurrentDirection);
};
