// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "TalesRunnerComponent.generated.h"

struct FMappableConfigPair;

struct FInputActionValue;
/*!
 * Commpont that sets up input and camera handling for player controlled pawns(or bots that simulate players)
 *
 * This depends on a PawnExtensionComponent to coordinate initialization
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class D_TALESRUNNER_REP_API UTalesRunnerComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
	
public:
	UTalesRunnerComponent(const FObjectInitializer& ObjectInitializer);

	//! Returns the hero component if one exists on the specified actor
	UFUNCTION(BlueprintPure, Category = "Tales|Hero")
	static UTalesRunnerComponent* FindRunnerComponent(const AActor* Actor){ return (Actor ? Actor->FindComponentByClass<UTalesRunnerComponent>() : nullptr); }

	// //! @return true this is controlled by a real player and processed for enough in init
	bool IsReadyToBindInputs() const;

	//! The name of the extension event sent via UGameFrameworkComponentManaager
	static const FName NAME_BindInputsNow;

	//! The name of this component-implemented feature
	static const FName NAME_ActorFeatureName;

	//! Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override{ return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//! End IGameFrameworkInitStateInterface interface

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> DefaultInputConfig;

	//! True when player input bindings have been applied, will never be true for non - players
	bool bReadyToBindInputs;
};
