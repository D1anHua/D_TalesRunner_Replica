﻿
#pragma once
#include "GameFramework/CharacterMovementComponent.h"
#include "TalesCharacterMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

class ATalesCharacter;
/*!
 * @name ECustomMovementMode
 * My Custom Movement Mode
 * In EngineTypes.h
 * @brief Slide Move mode
 */
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Prone			UMETA(DisplayName = "Prone"),
	CMOVE_Max			UMETA(Hidden),
};

/*!
 * @name UTalesCharacterMovementComponent
 * Try to process the Acceleration Replicated
 */
UCLASS(Config = Game)
class D_TALESRUNNER_REP_API UTalesCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	//! Sprint_MaxWalkSpeed
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Speed")
	float MaxSprintSpeed = 750.f;

	//! Slide Parameter
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float MinSlideSpeed = 100;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float MaxSlideSpeed = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float SlideEnterImpulse = 400.f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float SlideGravityForce = 4000;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float SlideFriction = .06f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float BrakingDecelerationSliding = 1000.f;

	//! Prone Parameter
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float ProneEnterHoldDuration = .4f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float ProneSlideEnterImpulse = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float ProneMaxSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Slide")
	float BrakingDecelerationProne = 2500.f;

	//! Dash Parameter
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Dash")
	float DashCooldownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Dash")
	float AuthDashCooldownDuration = .9f;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* DashMontage;

	//! Mantle
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	float MantleMaxDistance = 200;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	float MantleReachHeight = 50;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	float MantleMinDepth = 30;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	float MantleMinWallSteepnessAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	float MantleMaxSurfaceAngle = 40;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	float MantleMaxAlignmentAngle = 45;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* TallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* TransitionTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* ShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* TransitionShortMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* ProxyTallMantleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Montage")
	UAnimMontage* ProxyShortMantleMontage;
	// @TODO Really need ?	
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Mantle")
	uint8 MantleScanNumber = 6;
	
	UPROPERTY(Transient)
	ATalesCharacter* TalesCharacterOwner;
	
	//! Sprint begin Logical ---- network safely
	bool Safe_bWantToSprint;
	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToProne;
	bool Safe_bWantsToDash;
	bool Safe_bHadAnimRootMotion;
	bool Safe_bTransitionFinished;
	

	float DashStartTime;
	FTimerHandle TimerHandle_EnterProne;
	FTimerHandle TimerHandle_DashCooldown;
	TSharedPtr<FRootMotionSource_MoveToForce> TransitionRMS;
	UPROPERTY(Transient)
	UAnimMontage* TransitionQueuedMontage;
	float TransitionQueuedMontageSpeed;
	int TransitionRMS_ID;

public:
	// Replicated
	UPROPERTY(ReplicatedUsing = OnRep_DashStart) bool Proxy_bDashStart;
	UPROPERTY(ReplicatedUsing = OnRep_ShortMantle) bool Proxy_bShortMantle;
	UPROPERTY(ReplicatedUsing = OnRep_TallMantle) bool Proxy_bTallMantle;

	// Delegates
	UPROPERTY(BlueprintAssignable) FDashStartDelegate DashStartDelegate;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UFUNCTION() void OnRep_DashStart();
	UFUNCTION() void OnRep_ShortMantle();
	UFUNCTION() void OnRep_TallMantle();
	

public:
	// Initial Function
	UTalesCharacterMovementComponent();
	virtual void InitializeComponent() override;

	// Helper Function
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMocementMode) const;
	UFUNCTION(BlueprintPure)
	bool IsMovementMode(EMovementMode InMovementMode) const;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

private:
	// --------------------------------------- Slide Mode -------------------------------------------------
	//! Slide Helper Function
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	bool CanSlide()const;
	void PhysSlide(float deltaTime, int32 Iterations);

	// --------------------------------------- Prone Mode -------------------------------------------------
	//! Prone Helper Function
	void OnTryEnterProne(){Safe_bWantsToProne = true;};
	UFUNCTION(Server, Reliable) void Server_EnterProne();
	
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float deltaTime, int32 Iterations);

	// ---------------------------------------    Dash    -------------------------------------------------
	//! Dash Helper Function
	void OnDashCooldownFinished();
	bool CanDash() const;
	void DoDash();
	
	// --------------------------------------    Mantle    ------------------------------------------------
	bool TryMantle();
	FVector GetMantleStartLocation(FHitResult FrontHit, FHitResult SurfaceHit, bool bTallMantle) const;

private:
	bool IsServer() const;
	float CapR() const;
	float CapHH() const;
	
public:
	// --------------------------------------- Saved Mode -------------------------------------------------
	//! SaveMove snapshot
	class FSavedmove_Tales : public FSavedMove_Character
	{
		typedef  FSavedMove_Character Super;
	public:
		enum CompressedFlag
		{
			FLAG_Sprint    = 0x10,
			FLAG_Dash      = 0x20,
			FLAG_Custom_2  = 0x40,
			FLAG_Custom_3  = 0x80,
		};
		// :用来说明这个值只占一位
		//! CompressedFlags
		uint8 Saved_bWantsToSprint : 1;
		uint8 Saved_bWantsToDash   : 1;
		uint8 Saved_bPressedZippyJump : 1;

		//! To Save Some Variables
		uint8 Saved_bHadAnimRootMotion : 1;
		uint8 Saved_bTransitionFinished : 1;
		uint8 Saved_bPrevWantsToCrouch : 1;
		uint8 Saved_bWantsToProne      : 1;

		FSavedmove_Tales();
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
	
	// --------------------------------------- Network Prediction Data -------------------------------------------------
	class FNetworkPredictionData_Client_Tales : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Tales(const UCharacterMovementComponent& ClientMovement);
		typedef FNetworkPredictionData_Client_Character Super;
		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
public:
	// --------------------------------------- Trigger Data -------------------------------------------------
	// @TODO This Can Move to GAS
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
	UFUNCTION(BlueprintCallable)
	void CrouchPressed();
	UFUNCTION(BlueprintCallable)
	void CrouchReleased();
	UFUNCTION(BlueprintCallable)
	void DashPressed();
	UFUNCTION(BlueprintCallable)
	void DashReleased();
};



