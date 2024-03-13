
#pragma once
#include "GameFramework/CharacterMovementComponent.h"
#include "TalesCharacterMovementComponent.generated.h"
/*!
 * FTalesCharacterCroundInfo
 *
 * Information about the ground under the character
 */
USTRUCT(BlueprintType)
struct FTalesCharacterGroundInfo
{
	GENERATED_BODY()

	FTalesCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/*!
 * @name UTalesCharacterMovementComponent
 * Try to process the Acceleration Replicated
 */
UCLASS(Config = Game)
class D_TALESRUNNER_REP_API UTalesCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UTalesCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual void SimulateMovement(float DeltaTime) override;

	//! @return Current Ground info, Call this will update the ground info if it's out of data
	UFUNCTION(BlueprintCallable, Category = "Tales|CharacterMovement")
	const FTalesCharacterGroundInfo& GetGroundInfo();

	void SetReplicatedAcceleration(const FVector& InAcceleration);

	//! UMovementComponent Interface
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;
	//! End of UMovementComponent Interface

protected:
	// Cached ground info for the character, Do not access the directly
	FTalesCharacterGroundInfo CachedGroundInfo;

	// Transient: 暂时的,
	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;
};
