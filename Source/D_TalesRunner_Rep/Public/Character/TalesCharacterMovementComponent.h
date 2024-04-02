
#pragma once
#include "GameFramework/CharacterMovementComponent.h"
#include "TalesCharacterMovementComponent.generated.h"
/*!
 * @name UTalesCharacterMovementComponent
 * Try to process the Acceleration Replicated
 */
UCLASS(Config = Game)
class D_TALESRUNNER_REP_API UTalesCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UTalesCharacterMovementComponent();

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

public:
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
	UFUNCTION(BlueprintCallable)
	void CrouchPressed();

private:
	//! SaveMove snapshot
	class FSavedmove_Tales : public FSavedMove_Character
	{
		typedef  FSavedMove_Character Super;
		
	public:
		//! 用来说明这个值只占一位
		uint8 Saved_bWantsToSprint:1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Tales : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Tales(const UCharacterMovementComponent& ClientMovement);
		typedef FNetworkPredictionData_Client_Character Super;
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	
	//! Sprint begin Logical ---- network safely
	bool Safe_bWantToSprint;

	//! Sprint_MaxWalkSpeed
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Speed")
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Custom|Speed")
	float Walk_MaxWalkSpeed;
};
